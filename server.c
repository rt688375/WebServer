#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "server.h"
#include "tcp.h"
#include "errno.h"

#define TRUE 1
#define FALSE 0

void clientProccess(const int clientSocket);

int startServer(const unsigned int port){

	int serverSocket;
	int clientSocket;
	char clientIP[16];
	unsigned int clientPort;

	int pid;

	serverSocket = newTCPServerSocket4("0.0.0.0",port,5);
	if(serverSocket == -1){
		error(errno, "No se puede crear el Socket");
		return FALSE;
	}

	while(TRUE){
		clientSocket = waitConnection4(serverSocket, clientIP, &clientPort);

		pid = fork();
		if(pid == -1){
		close(clientSocket);
		continue;
		}
		else if(pid == 0){
			//Soy el hijo
			clientProccess(clientSocket);
		}
		else if(pid > 0){
			//Soy el padre
			close(clientSocket);
		}
	}
	
	return TRUE;

}


void clientProccess(const int clientSocket){

	char *buffer;
	char *firstLine;
	char html[250];
	int file;
	int readBytes;
	int firstFlag;
	char *s2;
	char *ptr;
	int len;
	int i;


	buffer = calloc(255,1);
	firstLine = calloc(255,1);
	firstFlag = TRUE;

	while(readTCPLine4(clientSocket, buffer, 254)>0){
		if(strcmp(buffer, "\r\n") == 0){
			break;
		}

		if(firstFlag){
			strcpy(firstLine, buffer);
			firstFlag = FALSE;
		}
		
		bzero(buffer, 255);

	}

	// Procesar el get
	
	ptr = strtok(firstLine, "/");
	ptr = strtok(NULL, " ");
		fprintf(stdout,"%s\n",ptr);


	
	//Responder con 200 ok si existe
	strcpy(html, "HTTP/1.1 200 ok\r\n");
	sendTCPLine4(clientSocket, html, strlen(html));
	strcpy(html, "Content-Type: text/html\r\n");
	sendTCPLine4(clientSocket, html,strlen(html));
	strcpy(html, "Content-Lenght: 171\r\n");
	sendTCPLine4(clientSocket, html,strlen(html));
	strcpy(html, "\r\n");
	sendTCPLine4(clientSocket, html, strlen(html));

	//Enviar el archivo
	file = open(ptr, O_RDONLY);

	//RESPONDER CON 404 NOT FOUND SI NO EXISTE
	if(file == -1){
		strcpy(html, "<html><head><title>404 NOT FOUND</title></head><body>NO EXISTE!!!</body></html>");
		sendTCPLine4(clientSocket, html,strlen(html));
	}

	else{
		while((readBytes = read(file,buffer, 255)) > 0){
			sendTCPLine4(clientSocket, buffer, readBytes);
		}
	}
	
	//CERRAMOS LA COMUNICACIÓN
	close(clientSocket);
	free(buffer);
	free(firstLine);
	
	return;
}
	


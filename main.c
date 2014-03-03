#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "server.h"

#define DEF_PORT 8080
#define MINPORT 1
#define MAXPORT 65000
#define TRUE 1
#define FALSE 0

//Prototipos
int processArgs(int argc, char *argv[]);
void printHelp(int is_error, char *cmd, char *error);

unsigned int port = DEF_PORT;

int main(int argc, char *argv[]){
	
	printf(".:: Small Hard Coded Web Server::.\n");
	if(!processArgs(argc, argv)){
		return 1;
	}

	startServer(port);

	return 0;
}

int processArgs(int argc, char *argv[]){

	int i;
	unsigned int temp;

	for(i=1; i < argc; i++){
		if(strcmp(argv[i], "-p") == 0){
			temp = strtoimax(argv[++i], NULL, 10);
			if(temp == 0 || temp < MINPORT || temp > MAXPORT){
				printHelp(TRUE, argv[0], "Puerto fuera de rango\n");
				return FALSE;
			}
			port = temp;
		}
		
		else if(strstr(argv[i], "-v")!=NULL){
			
		}
	}
	
	return TRUE;

}


void printHelp(int is_error, char *cmd, char *error) {
	
	if(is_error) {
		printf("Error: %s\n",error);
	}
		
	printf("USE:\t%s [OPTIONS]\n",cmd);
	printf("\nOPCIONES:\n");
	printf("\t-p\t Numero de Puerto (Default %u)\n",DEF_PORT);
	printf("\t-v[vvvv]\t Nivel de Verbosity\n");
	printf("\n");
	return;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "internas.h"
#include "entrada_minishell.h"
#include "ejecutar.h"


int main(int argc, char *argv[])
{

   char buf[BUFSIZ];
   char *orden;
   char *cadena;


   while (1)
   {
   
	imprimir_prompt();
	leer_linea_ordenes(buf);
	cadena = strdup(buf);
	while((orden = strsep(&cadena, ";")) !=0){	
		if(strcmp(orden, "exit") !=0){
		       if(es_ord_interna(orden)!=0){
			       ejecutar_ord_interna(orden);
			}
		       else{
			       ejecutar_linea_ordenes(orden);
		       }
		else{
			exit(1);
		}}
	}

	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
   }

   return 0;
}




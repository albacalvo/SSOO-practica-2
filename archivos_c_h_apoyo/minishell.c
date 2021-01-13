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

static void manejar_sigchild(int signo){  //Cuando uno de los procesos finaliza, se elimina al zombie
	int estado;
	waitpid(-1,&estado,WNOHANG); //Espera a que finalice el proceso y guarda su estado en ese momento.Para conseguir que el proceso no se bloquee utilizamos WHOHANG
}

int main(int argc, char *argv[])
{

	char buf[BUFSIZ]; //almacenenamos la dirección del puntero en ese momento
	char *orden;
	char *cadena;
	struct sigaction sa; //Para el manejo de señales creamos una estructura independiente

	memset(&sa,0,sizeof(sa)); //Nos permite variar el comportamiento de la señal
	sa.sa_handler = manejar_sigchild; //Recogemos los procesos que han muerto
	sa.sa_flags = SA_NOCLDSTOP | SA_RESTART; //Con la primera flag hacemos que no se nos devuelva nada cuando el hijo zombie muere. La segunda flag controla que sucede cuando se ejecutan determinadas acciones(open, read, write) para que el manejador de la señal vuelva a su estado normal

	sigaction(SIGCHLD, &sa, NULL);//Cuando recibe una determinada señal nos permite variar la accion que se lleva a cabo un proceso con esta funcion


   while (1)
   {
   
	imprimir_prompt();	 //Imprime el prompt por pantalla
	leer_linea_ordenes(buf);//Lee la orden introducida por le ususario
	cadena = strdup(buf);	//Crea un duplicado de la orden introducida por el usuario
	while((orden = strsep(&cadena, ";")) !=0){		//Extrae un elemento de la cadena
		if(strcmp(orden, "exit") !=0){			//Si es 0 ejecuta el exit
		       if(es_ord_interna(orden)!=0){		//Comprueba si es orden interna
			       ejecutar_ord_interna(orden);	//Ejecuta la orden interna
			}
		       else{
			       ejecutar_linea_ordenes(orden);	//Ejecuta la orden externa
		       }
		}
		else{
			exit(1);
		}
	}	   
   }

   return 0;
}




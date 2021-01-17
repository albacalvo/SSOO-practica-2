#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>


/* funcion que abre el archivo situado en la posicion indice_entrada+1 */
/* de la orden args y elimina de ella la redireccion completa          */
                                  
void redirec_entrada(char **args, int indice_entrada, int *entrada)
{
	*entrada =  open(args[indice_entrada+1],O_RDONLY); //O_RDONLY solo da el permiso de lectura en la redireccion de entrada
	
	if(*entrada==-1){  //Si hay un erro lo indica
		perror("Error al abrir el fichero");
	   	 
	}else{
		args[indice_entrada]=NULL; //Elimina la redireccion, es decir, el elemento del array args en la posicion indice_entrada
		args[indice_entrada+1]=NULL; //Elimina el fichero 
   		
   }	   
}

/* funcion que abre el archivo situado en la posicion indice_salida+1 */
/* de la orden args y elimina de ella la redireccion completa         */

void redirec_salida(char **args, int indice_salida, int *salida) 
{
	//Da los permiso de entrada y  escritura a la salida que se genera con open 	
	*salida =  open(args[indice_salida+1],O_RDWR | O_CREAT | O_TRUNC ,0777);
	if(*salida==-1){ 
		perror("Error al abrir el fichero");
		

	}else{
		args[indice_salida]=NULL;   // Elimina la redireccion de salida, es decir el elemneto del array args en la posicion indice_salida
	   	args[indice_salida+1]=NULL; // Elimina el fichero
	}

}


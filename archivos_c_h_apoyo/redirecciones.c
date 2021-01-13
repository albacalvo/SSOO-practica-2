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
	*entrada =  open(args[indice_entrada+1],O_RDONLY); //O_RDONLY solo otorga permisi de lectura para la redireccion de entrada
	
	if(*entrada==-1){ 
		perror("Error al abrir el fichero");
	   	 

	}else{
		args[indice_entrada]=NULL; 
		//Si no hay un error elimina la redireccion: el elemento del array en la posicion indice_entrada
		args[indice_entrada+1]=NULL; //Posterirormente elimina el fichero 
   		
   }	   

}

/* funcion que abre el archivo situado en la posicion indice_salida+1 */
/* de la orden args y elimina de ella la redireccion completa         */

void redirec_salida(char **args, int indice_salida, int *salida) 
{
	*salida =  open(args[indice_salida+1],O_RDWR | O_CREAT | O_TRUNC ,0777);  //Permisos de entrada y escritura a la salida generada con open

	//O_RDWR  -> permiso lectura y escritura. 
	//O_CREAT -> abre el fichero seleccionado. 
	//O_TRUNC -> abre el fichero y cambia o trunca la longitud de este a 0.
	
	if(*salida==-1){ 
		perror("Error al abrir el fichero");
		

	}else{
		args[indice_salida]=NULL;   // Elimina la redireccion de salida
	   	args[indice_salida+1]=NULL; // Elimina el fichero igual que en redirecciones de entrada	
	}

}


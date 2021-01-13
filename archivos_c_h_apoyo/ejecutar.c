#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"

int **crear_pipes(int n_ordenes){ //Se crean las tuberias necesarias
	int **pipes = NULL;
	int i;

	//Se asigna la memoria para las tuberias
	pipes = (int **)malloc(sizeof(int *) * (n_ordenes - 1));
	for(i = 0; i < (n_ordenes - 1); i++){
		pipes[i]=(int *)malloc(sizeof(int) * 2);
      		pipe(pipes[i]); //Se crea la tuberia
   }
    return (pipes);
}


pid_t ejecutar_orden(const char *orden, int entrada , int salida , int *pbackgr){

   char **args;
  pid_t pid;
  int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */
  
   if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL) //Se comprueba si no se ha introducido ninguna orden (si se pulsa ENTER), si es asi entra en el if  
   {
	
      return(-1);
   }

   /* Si la línea de órdenes posee tuberías o redirecciones, podrá incluir */
   /* aquí, en otras fases de la practica, el código para su tratamiento.  */


//Se comprueba si hay una redireccion de entrada para que invoque a la funcion redirec_entrada.
   if(indice_ent!=-1){ 
      redirec_entrada(args,indice_ent,&entrada);
   }

   //Hacemos lo mismo pero con la redireccion de salida
   if(indice_sal!=-1){ 
      redirec_salida(args,indice_sal,&salida);
   }

   
   pid=fork(); //Se crea un proceso hijo 

   if(pid==-1){
//Si ocurre algún error en la creacion del hijo lo indicará
   	perror("Error al llamar a la funcion fork");
   	exit(-1); 
   }else if(pid == 0){

//Se comprueba que la entrada se realiza desde un fichero si no es asi se cambiaria para que asi fuera
//A continuacion, creara una copia del archivo de entrada con STDIN_FILENO y se cierra la entrada.
      if(entrada!=0){
         dup2(entrada,STDIN_FILENO); //Duplica el archivo de entrada
         close(entrada);	    //Se cierra la entrada

      }
	if(salida!=1){ //El mismo procedimiento que con la entrada pero con la salida

         dup2(salida,STDOUT_FILENO); 
         close(salida);	            //Se cierra la salida

      }
   	if(execvp(args[0], args) == -1){   //En caso de que de error lo muestra y sale del programa.
   	    perror("Error al tratar de ejecutar");
   	    exit(-1);
   	}
   }
	if(entrada!=0){ //Si es distinto de 0 se cierra la entrada
         close(entrada);
   }
   	if (salida!=1){//Si es distinto de 1 se cierra la salida
         close(salida);
   }
      
   free_argumentos(args); 
   return(pid);	

}
void ejecutar_linea_ordenes(const char *orden)
{
   char **ordenes;
   int n_ordenes;
   pid_t *pids = NULL;
   int **pipes;
   int entrada,salida,i;
   int backgr;
   int estado;
   int opciones=0;

   ordenes=parser_pipes(orden,&n_ordenes); //Se convierte la cadena en una orden compuesta.

   pipes=crear_pipes(n_ordenes); //Cuando haya  n ordenes nos creará  n-1 tuberias

   //Ahora se asigna la memoria a los pids

   pids=(int *)malloc(sizeof(int) * n_ordenes);
   for(i=0;i<n_ordenes;i++){
      if(i==0){ //Si es la primera tuberia

         if(n_ordenes>1){ //Si hay mas tuberias
            salida=pipes[0][1];
         }
      else{ //si no hay tuberías
            salida = STDOUT_FILENO;
         }

     pids[0] = ejecutar_orden(ordenes[0], 0, salida, &backgr);

      } else if((i == n_ordenes-1)&&(n_ordenes>1)){//Si es la ultima tuberia y existen mas ordenes

     	entrada=pipes[n_ordenes-2][0];
        salida=STDOUT_FILENO;
     	pids[i]=ejecutar_orden(ordenes[i], entrada, salida, &backgr); //Ejecutamos la ultima orden existan o no tuberias

      } else {  //Si no es la primera ni la última tubería
     	salida=pipes[i][1];
	entrada=pipes[i-1][0];
	pids[i]=ejecutar_orden(ordenes[i], entrada, salida, &backgr);
     }
   }

if((backgr==0)&&(pids[n_ordenes-1] > 0)){//Si fuera  la última orden y no hubiera background
	
	 waitpid(pids[n_ordenes-1], &estado, opciones); //Hacemos que el padre espere la terminación de la minishell hija 
     }
   free_ordenes_pipes(ordenes,pipes,n_ordenes);//Liberamos la memoria
   free(pids);
}

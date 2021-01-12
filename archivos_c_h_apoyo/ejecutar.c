#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"

int **crear_pipes(int n_ordenes){ //Creamos las tuberias necesarias
	int **pipes = NULL;
	int i;

//Asignamos la memoria para las tuberias
	pipes = (int **)malloc(sizeof(int *) * (n_ordenes - 1));
	for(i = 0; i < (n_ordenes - 1); i++){
		pipes[i]=(int *)malloc(sizeof(int) * 2);
      		pipe(pipes[i]); //Por ultimo creamos la tuberia
   }
    return (pipes);
}


pid_t ejecutar_orden(const char *orden, int entrada , int salida , int *pbackgr)
{

   char **args;
  pid_t pid;
  int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */
  
   if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL) //Comprobamos si no se ha introducido ninguna orden, es decir si se ha pulsado ENTER, en caso afirmativo entraria en la condicion  
   {
	
      return(-1);
   }

   /* Si la línea de órdenes posee tuberías o redirecciones, podrá incluir */
   /* aquí, en otras fases de la practica, el código para su tratamiento.  */


//Comprobamos que haya una redireccion de entrada que llame a su funcion redirec_entrada.
   if(indice_ent!=-1){ 
      redirec_entrada(args,indice_ent,&entrada);
   }

   //Hacemos lo mismo que con la entrada pero con la salida, y entraria en la condicion
   if(indice_sal!=-1){ 
      redirec_salida(args,indice_sal,&salida);
   }

   
   pid=fork(); //creamos un proceso hijo 

   if(pid==-1){
//si ha ocurrido algun error en la creacion del hijo lo mostramos0
   	perror("Error al llamar a la funcion fork");
   	exit(-1); 
   }else if(pid == 0){
//Tenemos que comprobart que la entrada se realiza desde un fichero, en caso contrario se cambiaria para que asi fuera, posteriormente
//creamos una copia del archivo de entrada con STDIN_FILENO y cerramos la entrada.
      if(entrada!=0){
         dup2(entrada,STDIN_FILENO); //Cogemos el archivo de entrada y lo duplicamos
         close(entrada);//cerramos la entrada

      }
	if(salida!=1){ //Hacemos lo mismo que con la entrada, comprobamos que haya una salida desde fichero, si no fuera asi se cambiaria.

         dup2(salida,STDOUT_FILENO); 
//Igual que con la entrada pero con la salida
         close(salida);

      }
   	if(execvp(args[0], args) == -1){   //En casa de que la funcion ejecutase correctame, ejecutaria la orden y saldria del programa.
   	    perror("Error al tratar de ejecutar");//En caso de que diera error lo mostraria.
   	    exit(-1);
   	}
   }
	if(entrada!=0){ //Se debe cerrar la entrada si fuera distinto de 0
         close(entrada);
   }
   	if (salida!=1){//Lo mismo pasaria con la salida pero si fuera distinto de 1
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


      } else if( (i == n_ordenes-1) && (n_ordenes>1) ){//Si es la ultima tuberia y existen mas ordenes

     entrada=pipes[n_ordenes-2][0];
         salida=STDOUT_FILENO;
     pids[i]=ejecutar_orden(ordenes[i], entrada, salida, &backgr); //Ejecutamos la ultima orden existan o no tuberias

      } else {  //Si no es la primera ni la última tubería
     salida=pipes[i][1];
     entrada=pipes[i-1][0];
     pids[i]=ejecutar_orden(ordenes[i], entrada, salida, &backgr);

     }

   }


if((backgr==0) && (pids[n_ordenes-1] > 0) ){//Si fuera  la última orden y no hubiera background


       waitpid(pids[n_ordenes-1], &estado, opciones); //Hacemos que el padre espere la terminación de la minishell hija 
     }

   free_ordenes_pipes(ordenes,pipes,n_ordenes);//Liberamos la memoria
   free(pids);
}

   /* Si la orden es compuesta, podra incluir aqui, en otra fase de la */
   /* practica, el codigo para su tratamiento                          */


   //pid = ejecutar_orden(orden, &backgr);

    }             

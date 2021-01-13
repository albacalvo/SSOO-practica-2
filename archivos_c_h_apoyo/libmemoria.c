#include <stdlib.h>

#include "libmemoria.h"


void free_argumentos(char **args)	//libera de la memoria el array
{
   int i = 0;

   while(args[i])	//recorre todos los elementos del array y los libera uno a uno
      free(args[i++]);
   free(args);		//libera todas las posiciones reservadas en la memoria por el array
}

void free_ordenes_pipes(char **ordenes, int **fds, int nordenes) // una vez usados los pipes libera las ordenes de la memoria
{
   int i = 0;

   for (i = 0; i < nordenes; i++)	//recorre todas las ordenes las ordenes y las libera una a una
   {
      free(ordenes[i]);
      if (i < (nordenes - 1))
         free(fds[i]);		//libera de la memoria uno a uno los elementos de fds
   }

   free(ordenes);
   free(fds);
}

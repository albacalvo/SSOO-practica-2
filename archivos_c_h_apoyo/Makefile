CC= gcc
CFLAGS= -g -Wall

minishell: minishell.o entrada_minishell.o ejecutar.o libmemoria.o redirecciones.o libshell.a
	$(CC) $(CFLAGS) minishell.o ejecutar.o entrada_minishell.o libmemoria.o redirecciones.o -L./ -lshell -o minishell

minishell.o: minishell.c internas.h entrada_minishell.h ejecutar.h
	$(CC) $(CFLAGS) -c minishell.c

entrada_minishell.o: entrada_minishell.c entrada_minishell.h
	$(CC) $(CFLAGS) -c entrada_minishell.c

ejecutar.o: ejecutar.c ejecutar.h parser.h libmemoria.h redirecciones.h
	$(CC) $(CFLAGS) -c ejecutar.c

redirecciones.o: redirecciones.c redirecciones.h
	$(CC) $(CFLAGS) -c redirecciones.c

libmemoria.o: libmemoria.c libmemoria.h
	$(CC) $(CFLAGS) -c libmemoria.c

libshell.a: internas.o parser.o
	ar -rs libshell.a internas.o parser.o


.PHONY: clean
clean:
	rm -f minishell ejecutar.o minishell.o entrada_minishell.o redirecciones.o libmemoria.o

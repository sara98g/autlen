########################################################
CC=gcc
CFLAGS= -g -Wall
EJS = pr_afnd
########################################################


all: $(EJS) clear


pr_afnd: pr_afnd.o afnd.o palabra.o alfabeto.o estado.o vectorIndice.o relacion.o
	$(CC) $(CFLAGS) -o pr_afnd pr_afnd.o afnd.o palabra.o alfabeto.o estado.o vectorIndice.o relacion.o


pr_afnd.o: pr_afnd.c afnd.h
	$(CC) $(CFLAGS) -c pr_afnd.c

pr_relacion.o: pr_relacion.c relacion.h
	$(CC) $(CFLAGS) -c pr_relacion.c


relacion.o: relacion.c relacion.h
	$(CC) $(CFLAGS) -c relacion.c

alfabeto.o: alfabeto.c alfabeto.h
	$(CC) $(CFLAGS) -c alfabeto.c

estado.o: estado.c estado.h
	$(CC) $(CFLAGS) -c estado.c

palabra.o: palabra.c palabra.h
	$(CC) $(CFLAGS) -c palabra.c

vectorIndice.o: vectorIndice.c vectorIndice.h
	$(CC) $(CFLAGS) -c vectorIndice.c

afnd.o: afnd.c afnd.h
	$(CC) $(CFLAGS) -c afnd.c



run_afnd:
	./pr_afnd


valgrind_afnd:
	valgrind --leak-check=full ./pr_afnd


clear:
	rm -rf *.o

clean:
	rm -rf *.o $(EJS) *.dot

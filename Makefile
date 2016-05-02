COMPILADOR = gcc
ARQUIVOS = alocador.c shutdown.c substituidor.c usuario.c
OBJETOS = alocador.o shutdown.o substituidor.o usuario.o
DIRLIB = alocador.h shutdown.h util.h

all: linkar compilar limpar

linkar: $(ARQUIVOS)
	$(COMPILADOR) -c $(ARQUIVOS) $(DIRLIB)

compilar: $(ARQUIVOS) 
	$(COMPILADOR) $(OBJETOS) $(DIRLIB) -o trabso

limpar:
	rm -f *.o

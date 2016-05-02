DEPS = alocador.h substituidor.h util.h

trabSO: alocador.c shutdown.c substituidor.c usuario.c
     gcc  -o trabSO alocador.c shutdown.c substituidor.c usuario.c

shutdown.o: shutdown.c
    gcc -Wall -c shutdown.c

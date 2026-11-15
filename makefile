# makefile The Boys
# Carlos Maziero - DINF/UFPR, 2024/2

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g -std=c99 -I. -Ieventos
LDLIBS  = -lm
MAIN    = theboys
ENTREGA = $(MAIN)

# lista de arquivos de cabeçalho (a completar)
HDR = fila.h lef.h conjunto.h auxiliares.h entidades.h mundo.h eventos/eventos.h eventos/eventos_aux.h

# lista de arquivos-objeto (a completar)
# não inclua conjunto.o, senão ele será removido com "make clean"
OBJ = fila.o lef.o theboys.o mundo.o auxiliares.o eventos/eventos.o eventos/eventos_aux.o

# construir o executável
$(MAIN): $(MAIN).o $(OBJ) conjunto.o

# construir os arquivos-objeto (a completar)
$(MAIN).o: $(MAIN).c $(HDR)
$(OBJ): $(HDR)

# construir os TADs
fila.o: fila.c fila.h
lef.o: lef.c lef.h eventos/eventos.h

# executar
run: $(MAIN)
	./$(MAIN)

# testar no Valgrind
valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)

# gerar arquivo TGZ para entregar
tgz: clean
	-mkdir -p /tmp/$(USER)/$(ENTREGA)
	chmod 0700 /tmp/$(USER)/$(ENTREGA)
	cp -r *.c *.h conjunto.o makefile /tmp/$(USER)/$(ENTREGA)
	tar czvf $(ENTREGA).tgz -C /tmp/$(USER) $(ENTREGA)
	rm -rf /tmp/$(USER)
	@echo "Arquivo $(ENTREGA).tgz criado para entrega"

# limpar arquivos temporários
clean:
	rm -f *~ eventos/*~ $(OBJ) $(MAIN) /tmp/$(USER)/$(ENTREGA) $(ENTREGA).tgz


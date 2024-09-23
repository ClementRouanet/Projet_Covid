C = gcc -g -Wall
LIBS = -I include -lm
EXEC = covid19.exe

all : $(EXEC)

OBJS = obj/main.o obj/covid.o obj/population.o obj/hopital.o obj/graphe.o obj/arbre.o

covid19.exe : $(OBJS)
	$(C) $(OBJS) -o covid19 $(LIBS)

obj/main.o : src/main.c
	$(C) -c src/main.c -o obj/main.o $(LIBS)

obj/population.o : src/population.c include/population.h
	$(C) -c src/population.c -o obj/population.o $(LIBS)

obj/covid.o : src/covid.c include/covid.h
	$(C) -c src/covid.c -o obj/covid.o $(LIBS)

obj/hopital.o : src/hopital.c include/hopital.h
	$(C) -c src/hopital.c -o obj/hopital.o $(LIBS)

obj/graphe.o : src/graphe.c include/graphe.h
	$(C) -c src/graphe.c -o obj/graphe.o $(LIBS)

obj/arbre.o : src/arbre.c include/arbre.h
	$(C) -c src/arbre.c -o obj/arbre.o $(LIBS)


clean :
	@rm -f obj/*.o *.exe src/*~

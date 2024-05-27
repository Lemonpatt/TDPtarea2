FLAGS = -Wall -O2 -g

all: Graph.o MaxClique.o labtest

Graph.o: Graph.cpp Graph.h
	g++ $(FLAGS) -c Graph.cpp

MaxClique.o: MaxClique.cpp MaxClique.h
	g++ $(FLAGS) -c MaxClique.cpp

labtest: Graph.o MaxClique.o mainaux.cpp	
	g++ $(FLAGS) Graph.o MaxClique.o mainaux.cpp -o labtest

clean:
	rm -f *.o labtest

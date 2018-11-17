OBJS     = Main.o Graph.o Hashtable.o Lista.o 
SOURCE   = Main.cpp Graph.cpp Hashtable.cpp Lista.cpp 
HEADER   = Graph.hpp Hashtable.hpp Lista.hpp 
CC       = g++ 
OUT	= elegxos

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $@ $(OBJS) 

Main.o: Main.cpp
	$(CC) -c Main.cpp

Graph.o: Graph.cpp
	$(CC) -c Graph.cpp

Hashtable.o: Hashtable.cpp
	$(CC) -c Hashtable.cpp

Lista.o: Lista.cpp
	$(CC) -c Lista.cpp

clean:
	rm -f *.o $(OUT)

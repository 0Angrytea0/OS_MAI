SRC = main.cpp tree.cpp
OBJ = $(SRC:.cpp=.o)

SRC2 = client.cpp
OBJ2 = $(SRC2:.cpp=.o)

all: main client

main: $(OBJ)
	g++ -Wno-unused-variable $(OBJ) -o $@ -lrt -lzmq

client: $(OBJ2)
	g++ -Wno-unused-variable $(OBJ2) -o $@ -lrt -lzmq

.cpp.o:
	g++ -Wno-unused-variable -c $< -o $@

client.o: tree.hpp timer.hpp
main.o: tree.hpp
timer.o: timer.hpp
binary_tree.o: tree.hpp

clean:
	rm client.o main.o timer.o tree.o 

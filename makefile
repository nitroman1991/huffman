all: huff
huff: list.o engine.o tree.o
	g++-4.8 -std=c++11 -o huff list.o engine.o tree.o

tree.o:	tree.cpp tree.hpp
	g++-4.8 -std=c++11 -c tree.cpp

engine.o: engine.cpp engine.hpp tree.hpp
	g++-4.8	-std=c++11 -c engine.cpp

list.o:	list.cpp tree.hpp engine.hpp
	g++-4.8 -std=c++11 -c list.cpp

clean:
	rm -rf *o

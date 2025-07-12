tree_sim: main.o
	g++ main.o -o tree_sim

main.o: main.cpp binarysearchtree.h binarytree.h linkedtreenode.h tree.h
	g++ -c main.cpp

clean:
	rm  *.o tree_sim

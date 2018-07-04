CC = gcc
FLAGS = -Wall
OBJECTS = input_parse.o minisearch.o postingList.o sort.o trie.o trieInsertion.o trieOptions.o

all : main

main : $(OBJECTS)
	$(CC) $(FLAGS) -o minisearch input_parse.o minisearch.o postingList.o sort.o trie.o trieInsertion.o trieOptions.o -lm

input_parse.o : input_parse.c
	$(CC) $(FLAGS) -c input_parse.c -o input_parse.o

minisearch.o : minisearch.c
	$(CC) $(FLAGS) -c minisearch.c -o minisearch.o

postingList.o : postingList.c
	$(CC) $(FLAGS) -c postingList.c -o postingList.o

sort.o : sort.c
	$(CC) $(FLAGS) -c sort.c -o sort.o

trie.o : trie.c
	$(CC) $(FLAGS) -c trie.c -o trie.o

trieInsertion.o : trieInsertion.c
	$(CC) $(FLAGS) -c trieInsertion.c -o trieInsertion.o

trieOptions.o : trieOptions.c
	$(CC) $(FLAGS) -c trieOptions.c -o trieOptions.o -lm


.PHONY : clean
clean :
	rm $(OBJECTS) minisearch

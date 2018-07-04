#ifndef TRIEINSERTION_H
#define TRIEINSERTION_H

int nextWord(char* text,char** word,int start);
int createTrie(char** Doc,int lines,node** trie,int sorted);
void insertInTrie(node** trie,char* word,int id,int sorted);

#endif

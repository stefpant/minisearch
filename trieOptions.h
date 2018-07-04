#ifndef TRIEOPTIONS_H
#define TRIEOPTIONS_H

typedef struct query_info query_info;

int trieSearch(char** doc,int* docWC,int N,node* trie,char* option,
       int start,int avgdl,int kmax,int terminal_length,int sorted);
int trieDF(node* trieyo,char* option,int start,int sorted);
int trieTF(node* trieyo,char* option,int start,int sorted,int lines);

#endif

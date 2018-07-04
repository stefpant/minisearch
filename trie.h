#ifndef TRIE_H
#define TRIE_H

typedef struct node node;

node* nodeConstruct(char v);
void nodeDestruct(node* self);
void setValue(node* self,char v);
char getValue(node* self);
void setNext(node* self,node* next);
node* getNext(node* self);
void setPrevious(node* self,node* next);
node* getPrevious(node* self);
void setChild(node* self,node* child);
node* getChild(node* self);
int getCounterEPL(node* self);
void updateEPL(node* self,int id);
int getFrequencyIdEPL(node *self,int id);
void getIdsEPL(node *self,int **ids);

#endif

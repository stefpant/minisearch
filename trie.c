#include<stdio.h>
#include<stdlib.h>

#include"postingList.h"
#include"trie.h"

struct node{
	char value;
	node* next;
	node* child;
	ePostingList* myPL;
};

node* nodeConstruct(char v){
	node* newNode = malloc(sizeof( node));
	newNode->value=v;
	newNode->next=NULL;
	newNode->child=NULL;
	newNode->myPL=NULL;
//	newNode->myPL=ePostingListConstuct();
	return newNode;
}

void nodeDestruct(node* self){
	if(self==NULL) return;
	ePostingListDestruct(self->myPL);
	nodeDestruct(self->next);
	nodeDestruct(self->child);
	free(self);
	self=NULL;
	return;
}

void setValue(node* self,char v){
	self->value=v;
	return ;
}

char getValue(node* self){
	return self->value;
}

void setNext(node* self,node* next){
	self->next=next;
	return ;
}

node* getNext(node* self){
	return self->next;
}

void setChild(node* self,node* child){
	self->child=child;
	return ;
}

node* getChild(node* self){
	return self->child;
}


int getCounterEPL(node* self){
	if(self == NULL) return 0;
	if(self->myPL==NULL) return 0;
	return getCounter(self->myPL);
}

void updateEPL(node* self,int id){
	if(self->myPL==NULL) self->myPL=ePostingListConstuct();
	updatePL(&(self->myPL),id);
}

int getFrequencyIdEPL(node *self,int id){
	if(self==NULL) return 0;
	if(self->myPL==NULL) return 0;
	return getFrequencyIdPL(self->myPL,id);
}


void getIdsEPL(node *self,int **ids){
	if(self==NULL) return ;
	if(self->myPL==NULL) return ;
	if((*ids = malloc(getCounter(self->myPL)*sizeof(int)))==NULL){
		printf("Allocation Error!\n");
		return ;
	}
	getIdsPL(self->myPL,ids);
}

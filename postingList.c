#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"postingList.h"

struct ePostingList{
	int counter;//num of elements
	postingList *pl;//pl's first node
	postingList *last;//pl's last node
};

struct postingList{
	int id;
	int frequency;
	postingList *next;
};

ePostingList* ePostingListConstuct(){
	ePostingList* epl = malloc(sizeof(ePostingList));
	epl->counter=0;
	epl->pl = NULL;
	epl->last=NULL;
	return epl;
}

void ePostingListDestruct(ePostingList* self){
	if(self==NULL) return;
	postingListDestruct(self->pl);
	self->last = NULL;
	free(self);
	return;
}

void postingListDestruct(postingList* self){//recursive destruction
	if(self==NULL) return;
	postingListDestruct(self->next);
	free(self);
	return;
}

postingList* insertInPL(int id){//creates new pl's node
	postingList* pl = malloc(sizeof(postingList));
	pl->id=id;
	pl->frequency=1;
	pl->next=NULL;
	return pl;
}

int getCounter(ePostingList *self){
	return self->counter;
}

void updatePL(ePostingList **self,int id){
	if((*self)->pl==NULL){
		(*self)->pl=insertInPL(id);
		(*self)->last = (*self)->pl;
		(*self)->counter++;//new PL node
		return;
	}
	postingList *temp = (*self)->last;
	if(temp->id>id){
/*will never come there in trie's initialization but in case this happen
  search from the start the posting list and insert id sorted */
		temp = (*self)->pl;
		if(temp->id > id){//insert in start
			postingList *newpl = insertInPL(id);
			newpl->next = temp->next;
			(*self)->pl = newpl;
			(*self)->counter++;
			return ;
		}
		else if(temp->id==id){temp->frequency++;return;}
		//we are sure last->id > id so temp will never be NULL
		while((temp->next)->id < id){temp = temp->next;}
		if((temp->next)->id == id){
			(temp->next)->frequency++;
			return;
		}
		else{
			postingList *newpl = insertInPL(id);
			newpl->next = temp->next;
			temp->next = newpl;
			(*self)->counter++;
			return ;
		}
	}
	if(temp->id==id){
		temp->frequency++;
		return;
	}
	temp->next = insertInPL(id);
	(*self)->last = temp->next;
	(*self)->counter++;//new PL node
	return ;
}

int getFrequencyIdPL(ePostingList *self,int id){
	if(self->pl==NULL) return 0;
	postingList* temp=self->pl;
	while(1){
		if(temp->id==id) return temp->frequency;
		if(temp->id>id || (temp=temp->next)==NULL) return 0;
	}
}

//makes and returns an array with all IDs found in 'self' node
void getIdsPL(ePostingList *self,int **ids){
	if(self->pl==NULL) return ;
	postingList* temp=self->pl;
	int i=0;
	while(1){
		(*ids)[i++] = temp->id;
		if((temp=temp->next)==NULL) return ;
	}
}

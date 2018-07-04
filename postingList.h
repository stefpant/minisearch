#ifndef POSTINGLIST_H
#define POSTINGLIST_H

typedef struct postingList postingList;
typedef struct ePostingList ePostingList;

ePostingList* ePostingListConstuct();
void ePostingListDestruct(ePostingList* self);
void postingListDestruct(postingList* self);
int getCounter(ePostingList *self);
postingList* insertInPL(int id);
void updatePL(ePostingList **self,int id);
int getFrequencyIdPL(ePostingList *self,int id);
void getIdsPL(ePostingList *self,int **ids);

#endif

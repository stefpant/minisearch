#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"trie.h"

//gets a string text and a start pointer in text to tell us where to
//start searching the next word.Finds and saves this word and returns the
//'start' pointer where we will use to find the next word
//When found the last word returns -1
//if start is already -1 or we found an error then return -2 ->STOP
int nextWord(char* text,char** word,int start){
	if(text==NULL) return -2;//error
	if(start>strlen(text) || start<=-1) return -2;//error or END
	while(text[start]==' ' || text[start]=='\t') start++;//skip ws
	//now in text[start] starts the word which we return
	if(text[start]=='\0') return -2;//or maybe we reached '\0'
	int temp=start;//if not return the word
	while(text[temp]!=' ' && text[temp]!='\t' && text[temp]!='\0') temp++;
	if((*word = malloc((temp-start+1)*sizeof(char)))==NULL){
		printf("Allocation Error!\n");
		return -3;
	}
	strncpy(*word,text+start,temp-start);
	(*word)[temp-start] = '\0';
	while(text[temp]==' ' || text[temp]=='\t') temp++;//again skip ws
	if(text[temp]=='\0')//if after this word we reached the end
		return -1;
	return temp;//first char to check next time
}//after using the 'word' needs free!

void insertInTrie(node** trie,char* word,int id,int sorted){
	int i=0;//character's index
	char c = word[i];
	node* temp=*trie;
	node* prevN=NULL;
	node* newN=NULL;
	node* parent=NULL;
	int flag=1,flag2=1;
	//flag:if temp->next==NULL then create new next
	//frag2:if child==NULL skip the second while loop

	while(1){//for all word's chars(has at least 1 char)
		while(flag2){//for all childs in temp's list(temp -> next -> next -> ...)
			if(c==getValue(temp)){
				flag=0;
//printf("'%c' found in trie go down!\n",c);
				break;
			}
			else if((c<getValue(temp))&&sorted){
				flag=2;
				break;
			}
			if(getNext(temp)==NULL) {/*printf("'%c' not found create next node!\n",c);*/break;}
			prevN = temp;//keep previous node
			temp = getNext(temp);//search until c found in trie
		}//or we reached the end of temp's list
		if(flag==1){//c not found in trie and greater than any other value in list
			setNext(temp,nodeConstruct(c));
			//setParent(getNext(temp),getParent(temp));
//printf("Next made with value:'%c'\n",c);
			temp = getNext(temp);
			flag=0;//from now on just create childs
		}
		else if((flag==2)&&sorted){
			newN = nodeConstruct(c);
			if(prevN==NULL){
				if(parent!=NULL)
					setChild(parent,newN);
				else *trie = newN;//change root
			}
			else
				setNext(prevN,newN);
			//setParent(newN,getParent(temp));
			setNext(newN,temp);
			temp = newN;
			flag=0;
		}
		//now temp has the 'c' value so we check its child
		if((c=word[++i])=='\0'){
			updateEPL(temp,id);
//printf("reached the end update PL!\n");
			return ;
		}
		if(getChild(temp)!=NULL) {
			/*printf("Search down for next char='%c'!\n",c);*/
			prevN = NULL;//no previous node here
			flag=1;
			parent = temp;
			temp=getChild(temp);
		}
		else{
			flag2=0;//from now on until the end of the word
			setChild(temp,nodeConstruct(c));//run these 4 lines
//printf("Made child with value '%c'!\n",c);
			//setParent(getChild(temp),temp);
			temp = getChild(temp);
		}
	}
}

int createTrie(char** Doc,int lines,node** trie,int sorted){
	char* word=NULL;
	int start;
	for(int f=0;f<lines;f++){
		start=0;
		while((start = nextWord(Doc[f],&word,start))!=-2){
			insertInTrie(trie,word,f,sorted);//f=id
			free(word);
		}
	}
	return 0;
}

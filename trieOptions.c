#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"trie.h"
#include"trieInsertion.h"
#include"trieOptions.h"
#include"sort.h"

//helpful struct to save informations for the search
struct query_info{
	char* word;
	node* last;
};

//some part same with insertInTrie...
int searchWordInTrie(node* trie,char* word,int sorted){//for trieDF
	int i=0;//character's index
	char c = word[i];
	node* temp=trie;
	int flag=1;
	//flag:if temp->next==NULL then create new next

	while(1){//for all word's chars(has at least 1 char)
		while(1){//for all childs in temp's list(temp -> next -> next -> ...)
			if(c==getValue(temp)){
				flag=0;
				break;//check next char
			}
			else if((c<getValue(temp))&&sorted) break;
			if(getNext(temp)==NULL) return 0;//word doesn't exists in trie :(
			temp = getNext(temp);//search until c found in trie
		}//or we reached the end of temp's list
		if(flag)//c not found in trie...exiting
			return 0;

		//now temp has the 'c' value so we check its child
		if((c=word[++i])=='\0'){
			return getCounterEPL(temp);
		}
		if(getChild(temp)==NULL) return 0;//word not found
		temp=getChild(temp);//else continue with child
	}
}

//almost same with above function but searching
//the id after the word is found
//if word or id not found return 0
int searchWordIdInTrie(node* trie,char* word,int id,int sorted){//for trieTF
	int i=0;//character's index
	char c = word[i];
	node* temp=trie;
	int flag=1;
	//flag:if temp->next==NULL then create new next
	while(1){//for all word's chars(has at least 1 char)
		while(1){//for all childs in temp's list(temp -> next -> next -> ...)
			if(c==getValue(temp)){
				flag=0;
				break;//check next char
			}
			else if((c<getValue(temp))&&sorted) break;
			if(getNext(temp)==NULL) return 0;//word doesn't exists in trie :(
			temp = getNext(temp);//search until c found in trie
		}//or we reached the end of temp's list
		if(flag)//c not found in trie...exiting
			return 0;

		//now temp has the 'c' value so we check its child
		if((c=word[++i])=='\0'){
			return getFrequencyIdEPL(temp,id);
		}
		if(getChild(temp)==NULL) return 0;//word not found
		temp=getChild(temp);//else continue with child
	}
}

//prints all words in trie
void printAllWords(node* self,char** word,int length,int wlen){
	if(self==NULL) return ;
	if(length+1==wlen){(*word) = realloc((*word),2*wlen*sizeof(char)+100);wlen*=2;}
	(*word)[length-1] = getValue(self);//save the node's character here
	int freq;
	if((freq = getCounterEPL(self))!=0){//word found
		(*word)[length] = '\0';//add a '\0' and print it
		printf("%s %d\n",(*word),freq);
		//printf("--> %s %d\n",(*word),freq);
	}
	printAllWords(getChild(self),word,length+1,wlen);
	printAllWords(getNext(self),word,length,wlen);
	return ;
}


//here we need the pointer to last node of the 'word' to have
//access to its posting list
node* getWordLastNode(node* trie,char* word,int sorted){
	int i=0;//character's index
	char c = word[i];
	node* temp=trie;
	int flag=1;
	//flag:if temp->next==NULL then create new next

	while(1){//for all word's chars(has at least 1 char)
		while(1){//for all childs in temp's list(temp -> next -> next -> ...)
			if(c==getValue(temp)){
				flag=0;
				break;//check next char
			}
			else if((c<getValue(temp))&&sorted) break;
			if(getNext(temp)==NULL) return NULL;//word doesn't exists in trie :(
			temp = getNext(temp);//search until c found in trie
		}//or we reached the end of temp's list
		if(flag)//c not found in trie...exiting
			return NULL;
		//now temp has the 'c' value so we check its child
		if((c=word[++i])=='\0'){
			if(getCounterEPL(temp)==0) return NULL;//word found but is subword in trie
			return temp;
		}
		if(getChild(temp)==NULL) return NULL;//word not found
		temp=getChild(temp);//else continue with child
	}
}

double findIDF(int N,int Nid){
	double frac = (N-Nid+0.5)/(Nid+0.5);
	return log10(frac);
}

double findScoreI(int N,int id,node* self,int D,int avgdl){
	float k1=1.2,b=0.75;//constant vars for score
	int freq;
	if((freq = getFrequencyIdEPL(self,id))==0) return 0;
	return findIDF(N,getCounterEPL(self))*freq*(k1+1)/(freq+k1*(1-b+b*D/avgdl));
}

int mergeSArrays(int **A,int lA,int *B,int lB){
	if(lB==0) return lA;
	int p1=0,p2=0;
	int fcounter=0;
	int *final = malloc((lA+lB)*sizeof(int));//allocate for max elements
	while(1){
		if(p1==lA){while(p2!=lB) final[fcounter++] = B[p2++]; break;}
		if(p2==lB){while(p1!=lA) final[fcounter++] = (*A)[p1++]; break;}
		if((*A)[p1]<B[p2])//insert in final sorted
			final[fcounter++] = (*A)[p1++];
		else if((*A)[p1]>B[p2])
			final[fcounter++] = B[p2++];
		else{
			final[fcounter++] = (*A)[p1++];
			p2++;
		}
	}
	if(!fcounter) return 0;
	if(lA!=0) free(*A);
	free(B);
	*A = realloc(final,fcounter*sizeof(int));//in the end resize the array
	return fcounter;
}

//for every word finds the IDs in which it has showed up and merges the IDs with those
//of the ther words removing the duplicates and at last returns the array of all merged
//IDs and the number of those items(IDs)
void find_ids(query_info **q, int n, int** ids, int* numof_ids){
	int* tempids;
	getIdsEPL(q[0]->last,ids);//first array of ids
	*numof_ids = getCounterEPL(q[0]->last);//first num of ids
	for(int i=1;i<n;i++){
		getIdsEPL(q[i]->last,&tempids);//next array to merge with the previous
		*numof_ids = mergeSArrays(ids,*numof_ids,tempids,getCounterEPL(q[i]->last));
	}
	return ;
}

//counts all number's digits
int itol(int x){
	int counter=1;
	if(x<0) x*=-1;
	while((x=x/10)>0) counter++;
	return counter;
}

void printResult(int i,int id,double score,char* text,int term_l,
  query_info **q,int qc){
	int text_len = strlen(text);
	int temp_len = text_len,index=0,indexloc;
	char *indic;
	char *subtext;
	char *word;
	int flag=1;//if word not fit keep it for next rounf
	int flag2=1;//if the word found in query underscore it
	int start=0,current_len,starting_len=0;
	int first_sp = itol(i)+1+itol(id)+itol((int)score)+1+6+2+1;//i.(id)[score] text
	if(score<0) first_sp++;
	int cur_line=0;

	int cur_pos,eol_flag;

	while(temp_len>0){//for every sub-text that fits in terminal line..
		cur_line++;
		indexloc=0;
		cur_pos = first_sp;
		eol_flag = 0;
		current_len=term_l;

		indic = malloc((current_len+1)*sizeof(char));
		subtext = malloc((current_len+1)*sizeof(char));
		for(int x=0;x<first_sp;x++) indic[indexloc++]=' ';
		while(1){//WS---word---repeat
			while(text[index]==' ' || text[index]=='\t'){
				if(text[index]=='\t'){
					cur_pos+= (cur_pos % 8 ) ? (cur_pos % 8 ) : 8;//8 = tab's spaces
				}
				if(cur_pos>=current_len){eol_flag = 1;break;}
				//if(indexloc>first_sp)
				indic[indexloc++]=text[index++];//copy whitespace
				cur_pos++;
				//else index++;
			}
			if(eol_flag) break;
			if(flag){
				if((start = nextWord(text,&word,start))==-2) break;
				if(cur_pos+strlen(word)>current_len){
					flag=0;
					break;
				}
			}
			flag=1;
			for(int x=0;x<qc;x++){
				if(!strcmp(word,q[x]->word)){
					for(int y=0;y<strlen(word);y++)
						indic[indexloc++]='^';
					flag2=0;
					break;
				}
			}
			if(flag2){//word not in query fill it with ' '
				for(int y=0;y<strlen(word);y++)
					indic[indexloc++]=' ';
			}
			cur_pos+=strlen(word);
			if(flag){
				index+=strlen(word);
				free(word);
			}
			flag2=1;
		}
		//printline...
		indic[indexloc]='\0';
		strncpy(subtext,text+starting_len,index - starting_len);
		subtext[index - starting_len]='\0';
		if(cur_line==1)
			printf("%d.(%d)[%.4f] ",i,id,score);
		else{
			for(int g=0;g<first_sp;g++)
				printf(" ");
		}
		printf("%s\n%s\n",subtext,indic);
		free(indic);
		free(subtext);
		temp_len = text_len - index;
		while(text[index]==' ' || text[index]=='\t') index++;//for next line skip ws before first word
		starting_len=index;
	}
}

int trieSearch(char** doc,int* docWC,int N,node* trie,char* option,
  int start,int avgdl,int kmax,int terminal_length,int sorted){
	if(start==-1){//error
		printf("Wrong syntax for /search command\n");
		return -1;
	}
	query_info **q = malloc(10*sizeof(query_info*));
	int qcounter=0;
	while(start!=-1 && qcounter<10){//for every word save their info(word,node to PL )
		q[qcounter] = malloc(sizeof(query_info));
		start = nextWord(option,&(q[qcounter]->word),start);//save word
		q[qcounter]->last = getWordLastNode(trie,q[qcounter]->word,sorted);//and node to PL
		qcounter++;
	}
	q = realloc(q,qcounter*sizeof(query_info*));
	int *ids;
	int numof_ids=0;
	find_ids(q,qcounter,&ids,&numof_ids);
	if(numof_ids==0){//free and exit
		for(int i=0;i<qcounter;i++){
			free(q[i]->word);
			free(q[i]);
		}
		free(q);
		printf("No document found!\n");
		return 0;
	}
	double *score = malloc(numof_ids*sizeof(double));
	for(int j=0;j<numof_ids;j++){//find score for every id
		score[j]=0.0;
		for(int i=0;i<qcounter;i++){//for all words in query
			score[j] += findScoreI(N,ids[j],q[i]->last,docWC[ids[j]],avgdl);
		}
		//printf("(%d)->score=%f\n",ids[j],score[j]);
	}
	quickSort(&score,&ids,0,numof_ids-1);//sort score
	int kc=1;//count printed texts
	for(int i=numof_ids-1;i>=0&&kc<=kmax;i--){//print k texts with best score
		printResult(kc,ids[i],score[i],doc[ids[i]],terminal_length,q,qcounter);
		kc++;//or all found texts if num_of_ids<k
	}
	//free allocated memory
	for(int i=0;i<qcounter;i++){
		free(q[i]->word);
		q[i]->word = NULL;
		q[i]->last = NULL;
		free(q[i]);
	}
	free(q);
	free(ids);
	free(score);
	return 0;
}

int trieDF(node* trie,char* option,int start,int sorted){
	int result;
	char* word;
	int flag=1;
	//if flag=1 after the loop then print frequency for all words
	while((start = nextWord(option,&word,start))!=-2){
		flag=0;//if /df had at least one word
		result = searchWordInTrie(trie,word,sorted);
		printf("%s %d\n",word,result);
		free(word);
	}
	if(flag){
		word = calloc(2,sizeof(char));
		word[0]=word[1]='\0';
		printAllWords(trie,&word,1,2);
		free(word);
	}
	return 0;
}

int trieTF(node* trie,char* option,int start,int sorted,int lines){
	int id,result;
	char* word;
	start = nextWord(option,&word,start);
//---------------------checks syntax for tf-----------
	if(start==-2) {
		printf("Wrong command syntax!Try again!\n");
		printf("{Syntax for /tf: /tf [id] [word] }\n");
		return -1;
	}
	id = atoi(word);
	if(id==0 ){//id not a number
		int wordlen = strlen(word);
		for(int h=0;h<wordlen;h++){
			if(word[h]!='0'){
				printf("Id must be integer!Try again!\n");
				free(word);
				return -1;
			}
		}
	}
	else if(id<0 || id>=lines){
		printf("try with id in range [0,%d]\n",lines-1);
		return -1;
	}
	free(word);
	start = nextWord(option,&word,start);
	if(start!=-1) {
		printf("Wrong command syntax!Try again!\n");
		printf("{Syntax for /tf: /tf [id] [word] }\n");
		return -1;
	}
//---------------------------until here---------------------
	result = searchWordIdInTrie(trie,word,id,sorted);
	printf("%d %s %d\n",id,word,result);
	free(word);
	return 0;
}

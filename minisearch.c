#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<time.h>

#include"input_parse.h"
#include"trie.h"
#include"trieInsertion.h"
#include"trieOptions.h"

#define KNRM  "\x1B[0m"
#define KCYN  "\x1B[36m"

int main(int argc,char* argv[]){
	int i;
	char **DocContent=NULL;
	int k=10,lines=0;
	int sorted=0;//flag to sort or not the trie
	clock_t st=clock();
	if(input_parse(argc,argv,&DocContent,&k,&lines,&sorted) != 0){
		printf("Error in file parsing!\n");//wrong or empty input file
		for(int v=0;v<lines;v++){//delete created map until now
			free(DocContent[v]);//and then exit
			DocContent[v] = NULL;
		}
		free(DocContent);
		DocContent = NULL;
		return -1;
	}
	//printf("lines:%d,k:%d\n",lines,k);

	node* trieyo=nodeConstruct(DocContent[0][0]);//init trie with first char
	createTrie(DocContent,lines,&trieyo,sorted);
	clock_t end=clock();
	float tm=(float)(end-st)/CLOCKS_PER_SEC;
	printf("Trie just created( in %f seconds)!\n",tm);
	int start,sum=0;
	char *word;
	int* doc_wc = malloc(lines*sizeof(int));
	for(i=0;i<lines;i++){//finds how many words have every text in document
		doc_wc[i]=0;
		start=0;
		while((start = nextWord(DocContent[i],&word,start))!=-2){
			doc_wc[i]++;
			free(word);
		}
		sum+=doc_wc[i];
	}
	double avgdl=(double)sum/lines;

	char *option;

	struct winsize w;//finding terminal's length
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);//w.ws_col
	int terminal_length = w.ws_col;

	while(1){//loop for commands
		printf(KCYN "$>" KNRM);//colorized
		if(scanf(" %m[^\n]s",&option)!=1) continue;//read until '\n'
		start = nextWord(option,&word,0);//get command
		if(!strcmp(word,"/search") || !strcmp(word,"\\search")){
			free(word);
			trieSearch(DocContent,doc_wc,lines,trieyo,option,
				start,avgdl,k,terminal_length,sorted);//start=8
		}
		else if(!strcmp(word,"/df") || !strcmp(word,"\\df")){
			free(word);
			trieDF(trieyo,option,start,sorted);//start=4
		}
		else if(!strcmp(word,"/tf") || !strcmp(word,"\\tf")){
			free(word);
			trieTF(trieyo,option,start,sorted,lines);//start=4
		}
		else if(!strcmp(word,"/exit") || !strcmp(word,"\\exit")){
			free(word);
			free(option);
			break;//break loop to free structs and exit
		}
		else{
			printf("Unknown command.Try again...\n");
			free(word);
		}
		free(option);
	}


//free space
	//recursively delete from root all trie's nodes
	nodeDestruct(trieyo);
	for(i=0;i<lines;i++)//delete map
		free(DocContent[i]);
	free(DocContent);

	free(doc_wc);
	return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int doc_parse(char** DocContent[],char* docfile,int *lines){
	char c,pr_c;
	int Ccount=0;
	int i=0;
	FILE* fp;
	*lines = 0;
	int current_len = 2;
	if((fp=fopen(docfile,"r"))==NULL){
		printf("Given file doesn't exists!\n");
		return -1;
	}
	if(fp == NULL ){fclose(fp); return -1;}//error
	while((c=getc(fp)) != EOF){//1st parsing to find the lines
		if(c == '\n' && pr_c != '\n') (*lines)++;
		pr_c = c;
	}
        if(!(*lines)){fclose(fp); return 0;}//empty file exit
	*DocContent = malloc((*lines)*sizeof(char*));
	if((*DocContent)==NULL){
		*lines = 0;
		return -1;
	}
	printf("lines:%d\n",*lines);
		//return fp at the beginning of file
	int flag=1;//to skip the ids
	if(fseek(fp,0L,SEEK_SET) !=0) return -2;
	(*DocContent[0]) = malloc(2*sizeof(char));
	while((c=getc(fp)) != EOF){//2nd parsing to check ids and save text
		if(flag){//check id and skip whitespaces
			if(c==' ' || c=='\t' || c=='\n') {while((c=getc(fp))==' ' || c=='\t' || c=='\n') continue;}//skip whitespaces
			if(c == EOF ) break;
			fseek(fp,-1,SEEK_CUR);//get back last char
			int ch_id;
			fscanf(fp,"%d",&ch_id);//read word as integer
			if(i != ch_id ){
				*lines = i+1;//lines allocated untill now(free them in main)
				fclose(fp);
				return -3;//error wrong id
			}
			while((c=getc(fp))==' ' || c=='\t') continue;//skip whitespaces
			if(c == EOF || c == '\n'){//id with empty line
				*lines = i+1;
				fclose(fp);
				return -2;//error last id without text
			}
			flag=0;
		}
		Ccount++;
		if(Ccount > current_len){//if text continues but we don't have free space
			current_len*=2;//then reallocate with the double size
			(*DocContent)[i] = realloc((*DocContent)[i],current_len*sizeof(char));
		}
		if(c == '\n'){
			(*DocContent)[i][Ccount-1] = '\0';//text ends here and then resize the text
			(*DocContent)[i] = realloc((*DocContent)[i],Ccount*sizeof(char));
			if(i == (*lines)-1) break;//last line->stop
			current_len = 2;//preparations for next line
			(*DocContent)[++i] = malloc(2*sizeof(char));
			if(((*DocContent)[i-1]) == NULL){
				*lines = i;
				fclose(fp);
				return -1;
			}
			Ccount=0;
			flag=1;
		}
		else (*DocContent)[i][Ccount-1] = c;
	}
	fclose(fp);
	return 1;
}

//check arguments and for the given document as input initializes the map(DocContent)
int input_parse(int argc,char* argv[],char** DocContent[],int* k,int *lines,int *sorted){
	char *docfile=NULL;
	int i;
	int flag=0;
	for(i=1;i<argc;i++){
		if(strcmp(argv[i],"-i")==0){//then next argument must be the input file
			if(++i==argc){//error if nothing next
				printf("Missing input file...\n");
				return -2;
			}
			//needs the '-i' argument to success else we haven't got the input file
			//and return failure
			flag = 1;
			docfile = malloc(strlen(argv[i])+1);
			strcpy(docfile,argv[i]);
			printf("docfile = %s\n",docfile);
		}
		else if(strcmp(argv[i],"-k")==0){//if found this flag change the k
			if(i+1==argc){//if value for k missing just go on with k=10
				printf("Value for k not found.Continue with default value(k=10)\n");
				break;
			}
			*k = atoi(argv[++i]);
			if(!(*k)){//fails if k=0 or argument not a number
				printf("Try again with k!=0\n");
				free(docfile);
				return -2;
			}
		}
		else if(strcmp(argv[i],"-sorted")==0){
			*sorted = 1;//if found this flag sort the trie
		}
		else{//for every other argument/flag or whatever
			printf("Unrecognized arguments!\n");
			free(docfile);
			return -2;//exits 
		}
	}
	if(!flag){
		printf("Input file not found!\n");
		return -2;
	}
	int error = doc_parse(DocContent,docfile,lines);//mapping here
	free(docfile);
	if(error<=0) return -1;//error
	return 0;//finally success
}

#include<stdio.h>
#include<stdlib.h>

int partfunc(double** A,int** B,int left,int right,double pivot){
  int leftP = left,rightP = right-1;
  double temp;
  int ftemp;
  while(1){
    while((*A)[leftP] < pivot && leftP<right) leftP++;
    while((*A)[rightP] > pivot && rightP>left) rightP--;

    if(leftP>=rightP) break;
    else{
      temp = (*A)[leftP];
      (*A)[leftP] = (*A)[rightP];
      (*A)[rightP] = temp;
      ftemp = (*B)[leftP];
      (*B)[leftP++] = (*B)[rightP];
      (*B)[rightP--] = ftemp;
    }
  }
  temp = (*A)[leftP];
  (*A)[leftP] = (*A)[right];
  (*A)[right] = temp;
  ftemp = (*B)[leftP];
  (*B)[leftP] = (*B)[right];
  (*B)[right] = ftemp;
  return leftP;
}

void quickSort(double** A,int** B,int left,int right){//sort A,B following the sorting
	if(right-left<=0) return ;
	double pivot = (*A)[right];
	int partition = partfunc(A, B, left, right, pivot);
	quickSort(A, B, left, partition-1);
	quickSort(A, B, partition+1, right);
}

#include <stdio.h>
#include <string.h>
#include "optab.h"
#include "assembler.h"

typedef struct _symtab{
	char symbol[100]; 
	uint16_t addr; 	
} symtab;

typedef struct _field{
	char label[7]; 
	char operation[7]; 
	char operands[10]; 
	char comment[100]; 
	uint16_t loc; 
	unsigned onj; 
} field; 

symtab sym[1000]; 
field ins[1000]; 

void preprocess(char **sic, int n){
	// 	To make each line to be four columns. 
	int i, j, count; 
	uint16_t loc; 
	char *ptr, tmp[10][100]; 

	ptr = strtok(sic[0], " "); 
	strcpy(ins[0].label, ptr); 
	ptr = strtok(NULL, " "); 
	strcpy(ins[0].operation, ptr); 
	ptr = strtok(NULL, " "); 
	strcpy(ins[0].operands, ptr); 
	ptr = strtok(NULL, " "); 
	if(ptr!=NULL)
		strcpy(ins[0].comment, ptr); 
	
	count = atoi(ins[0].operands); 
	loc = (count/1000)*16*16*16 + ((count%1000)/100)*16*16 + ((count%100)/10)*16 + count%10; 

	for(i=1;i<n;i++){
		// Give the loc
		ins[i].loc = loc+3; 
		loc += 3; 

		// Split the Source Code
		count = 0; 
		ptr = strtok(sic[i], " "); 
		while(ptr!=NULL){
			strcpy(tmp[count++], ptr); 
			ptr = strtok(NULL, " "); 
		}
		strcpy(tmp[count], ptr); 

		for(j=0;j<26;j++){
			if(strcmp(optab[j], tmp[0])){
						
			}
		}


	}
	



}

void pass1(int n){
	//	Scanning first time. 
	//		1. To give all instruction Local Address. 
	//		2. To create symbol table. 	

	int i; 

	for(i=0;i<n;i++){
			
	
	}

}

void pass2(){
	
}

void assembler(char **sic, int n){

	preprocess(sic, n); 
	pass1(n); 
	
}

void toTarget(FILE *fout){
	
}

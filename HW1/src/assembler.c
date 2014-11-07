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
field ins[1000] = {}; 

void preprocess(char **sic, int n){
	// 	To make each line to be four columns. 
	int i, j, count; 
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
	
	for(i=1;i<n;i++){
		// Split the Source Code
		count = 0; 
		ptr = strtok(sic[i], " "); 
		while(ptr!=NULL){
			strcpy(tmp[count++], ptr); 
			ptr = strtok(NULL, " "); 
		}
		strcpy(tmp[count], ptr); 


		// Store to ins (incomplete)
		for(j=0;j<26;j++){
			if(strcmp(optab[j], tmp[0])){
				strcpy(ins[i].operation, tmp[0]); 
				if(tmp[1]==NULL)
					break;
				strcpy(ins[i].operands, tmp[1]); 

			}
		}
		if(ins[i].operation[0]!=0){
			strcpy(ins[i].label, tmp[0]); 
			strcpy(ins[i].operation, tmp[1]); 
		}
	}
}

void pass1(int n){
	//	Scanning first time. 
	//		1. To give all instruction Local Address. 
	//		2. To create symbol table. 	

	int i, count; 
	uint16_t loc; 

	count = atoi(ins[0].operands); 
	loc = (count/1000)*16*16*16 + ((count%1000)/100)*16*16 + ((count%100)/10)*16 + count%10; 

	count = 0; 
	for(i=1;i<n;i++){
		// Give the loc
		ins[i].loc = loc;
		if(strcmp(ins[i].operation, "BYTE")){

		}
		else if(strcmp(ins[i].operation, "WORD")){

		}
		else
			loc += 3;

		// Store into symbol table
		if(strcmp(ins[i].operation, "BYTE") || strcmp(ins[i].opearion, "WORD") || strcmp(ins[i].operation, "RESW") || strtmp(ins[i].operation, "RESB")){
			sym[count].addr = ins[i].loc; 
			strcpy(sym[count].symbol, ins[i].operation); 
		}
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

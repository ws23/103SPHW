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
	unsigned obj; 
} field; 

symtab sym[1000]; 
field ins[1000]; 

void showIns(){
	int i; 
	for(i=0;ins[i].operation;i++){
		if(ins[i].label!=NULL)
			printf("%10s ", ins[i].label); 
		else
			printf("%10s ", ""); 
		if(ins[i].operation!=NULL)
			printf("%10s ", ins[i].operation);
		else
			printf("%10s ", ""); 
		if(ins[i].operands!=NULL)
			printf("%10s ", ins[i].operands); 
		else
			printf("%10s ", ""); 
		printf("%6u %6u\n", ins[i].loc, ins[i].obj);
	}
}

void showSym(){
	int i; 
	for(i=0;sym[i].symbol;i++)
		printf("%10s %u\n", sym[i].symbol, sym[i].addr); 	
}

void addCode(char sic[]){
	
}

void preprocess(char *sic[], int n){
	// 	To make each line to be four columns. 
	int i, j, count; 
	char *ptr, tmp[10][100]; 	

	printf("XD1\n"); 
	ptr = strtok(sic[0], " "); 
	strcpy(ins[0].label, ptr); 
	ptr = strtok(NULL, " "); 
	strcpy(ins[0].operation, ptr); 
	ptr = strtok(NULL, " "); 
	strcpy(ins[0].operands, ptr); 
	ptr = strtok(NULL, " "); 
	if(ptr!=NULL)
		strcpy(ins[0].comment, ptr); 
	
	printf("XD2\n"); 
	for(i=1;i<n;i++){
		if(sic[i][0]=='.'){
			strcpy(ins[i].operation, "."); 
			continue; 
		}

		// Split the Source Code
		count = 0; 
		ptr = strtok(sic[i], " "); 
		while(ptr!=NULL){
			strcpy(tmp[count++], ptr); 
			ptr = strtok(NULL, " "); 
		}
		strcpy(tmp[count], ptr); 


		// Store to ins
		for(j=0;j<26;j++){
			if(strcmp(op[j].code, tmp[0])){
				strcpy(ins[i].operation, tmp[0]); 
				if(strcmp(ins[i].operation, "RSUB")){
					if(tmp[1]!=NULL)
						strcpy(ins[i].comment, tmp[1]); 
					break;	
				}
				if(tmp[1]==NULL)
					break;
				strcpy(ins[i].operands, tmp[1]); 
				if(tmp[2]==NULL)
					break;
				strcpy(ins[i].operands, tmp[2]); 
			}
		}
		if(ins[i].operation[0]!=0){
			strcpy(ins[i].label, tmp[0]); 
			strcpy(ins[i].operation, tmp[1]); 
		}
	}
	printf("XD3\n"); 
}

void pass1(int n){
	//	Scanning first time. 
	//		1. To give all instruction Local Address. 
	//		2. To create symbol table. 	

	int i, j, count, len; 
	uint16_t loc;
	char tmp[100], *ptr;  

	count = atoi(ins[0].operands); 
	loc = (count/1000)*16*16*16 + ((count%1000)/100)*16*16 + ((count%100)/10)*16 + count%10; 

	count = 0; 
	for(i=1;i<n;i++){
		// Give the loc
		ins[i].loc = loc;
		if(strcmp(ins[i].operation, "BYTE")){
			strcpy(tmp, ins[i].operands); 
			ptr = strtok(tmp, "'"); 
			ptr = strtok(NULL, "'"); 
			len = strlen(ptr); 
			loc += len/2; 
		}
		else
			loc += 3;

		// Store into symbol table
		if(strcmp(ins[i].operation, "BYTE") || strcmp(ins[i].operation, "WORD") || strcmp(ins[i].operation, "RESW") || strcmp(ins[i].operation, "RESB")){
			sym[count].addr = ins[i].loc; 
			strcpy(sym[count].symbol, ins[i].operation); 
		}
	}
}

void pass2(int n){
	//	Scanning second times. 
	//		1. To complete the object code. 	
	int i, j, count, len;
	char tmp[100], *ptr;
	 
	 

	for(i=0;i<n;i++){
		if(strcmp(ins[i].operation, "."))
			continue;
	
		// BYTE, WORD
		if(strcmp(ins[i].operation, "BYTE")){
			strcpy(tmp, ins[i].operands); 
			ptr = strtok(tmp, "'"); 
			ptr = strtok(NULL, "'");
			sscanf(ptr, "%X", &ins[i].obj);  						
		}
		else if(strcmp(ins[i].operation, "WORD")){
			strcpy(tmp, ins[i].operands); 
			ptr = strtok(tmp, "'"); 
			if(ptr==NULL)
				count = sscanf(ptr, "%X", &ins[i].obj); 
			else{
				ptr = strtok(NULL, "'"); 
				len = strlen(ptr); 
				for(j=len-1;j>=0;j--)
					ins[i].obj += (ptr[j]<<(len-j-1)); 	
			}	
		}

		// RESW, RESB
		if(strcmp(ins[i].operation, "RESW") || strcmp(ins[i].operation, "RESB"))
			continue; 	

		// get opcode
		for(j=0;j<26;j++){
			if(strcmp(ins[i].operation, op[j].code)){
				ins[i].obj += (op[j].opcode<<16); 
				break;	
			}
		}

		// get X
		strcpy(tmp, ins[i].operands); 
		ptr = strtok(tmp, ","); 
		ptr = strtok(NULL, ","); 
		if(strcmp(ptr, "X"))
			ins[i].obj += (1<<15); 	

		// get address	
		for(j=0;sym[j].symbol;j++){
			if(strcmp(ins[i].operands, sym[j].symbol)){
				ins[i].obj += sym[j].addr; 	
				break;
			}
		}
	}
}

void assembler(int n){
	
	memset(sym, 0, sizeof(sym)); 
	memset(ins, 0, sizeof(ins)); 

	printf("1\n"); 	
	preprocess(sic, n);
	printf("2\n"); 
	pass1(n); 
	printf("3\n"); 
	showIns(); 
	printf("4\n"); 
	pass2(n); 
	printf("5\n"); 	
}

void toTarget(FILE *fout){
	; 	
}

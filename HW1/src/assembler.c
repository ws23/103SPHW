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
int N, sN, start; 


void showIns(){
	int i, len; 
	char tmp[100], *ptr; 
	for(i=0;i<N;i++){
		if(ins[i].label!=NULL)
			printf("%-10s | ", ins[i].label); 
		else
			printf("%-10s | ", ""); 
		if(ins[i].operation!=NULL)
			printf("%-10s | ", ins[i].operation);
		else
			printf("%-10s | ", ""); 
		if(ins[i].operands!=NULL)
			printf("%-10s | ", ins[i].operands); 
		else
			printf("%10s | ", ""); 

		if(!strcmp(ins[i].operation, ".") || !strcmp(ins[i].operation, "END"))
			printf("       | \n"); 
		else if(!strcmp(ins[i].operation, "START") || !strcmp(ins[i].operation, "RESW") || !strcmp(ins[i].operation, "RESB"))
			printf("  %04X | \n", ins[i].loc);
		else if(!strcmp(ins[i].operation, "BYTE")){
			strcpy(tmp, ins[i].operands); 
			ptr = strtok(tmp, "'");
			if(!strcmp(ptr, "X")){ 
				ptr = strtok(NULL, "'");
				len = strlen(ptr) ; 
				printf("  %04X | %0*X\n", ins[i].loc, len, ins[i].obj); 
			}
			else if(!strcmp(ptr, "C")){
				ptr = strtok(NULL, "'")	; 
				len = strlen(ptr); 
				printf("  %04X | %0*X\n", ins[i].loc, len*2, ins[i].obj); 
			}
		}
		else 
			printf("  %04X | %06X\n", ins[i].loc, ins[i].obj);
	}
}

void showSym(){
	int i; 
	for(i=0;i<sN;i++)
		printf("%-10s | %06X\n", sym[i].symbol, sym[i].addr); 	
}

void addCode(char sic[]){
	// To make the SIC Code to be four columns. 
	int j, count; 
	char *ptr, *cpy, tmp[10][100]; 
	static int i = 0, first = 0; 

	memset(tmp, 0, sizeof(tmp)); 

	// comment
	if(sic[0]=='.'){
		strcpy(ins[i].operation, ".");
		i++;
		return;
	}


	// START
	if(!first){ 
		ptr = strtok(sic, " \t");
		cpy = ptr;
		ptr = strtok(NULL, " \t"); 
		strcpy(ins[i].label, cpy); 
		cpy = ptr; 
		ptr = strtok(NULL, " \t"); 
		strcpy(ins[i].operation, cpy); 
		cpy = ptr; 
		ptr = strtok(NULL, " \t"); 
		strcpy(ins[i].operands, cpy); 
		if(ptr!=NULL)
			strcpy(ins[i].comment, ptr);
		first = 1;
		start = i; 
		i++; 
	
		return; 
	}

	// Others
		// To split the Source Code
	count = 0; 
	ptr = strtok(sic, " \t");
	while(ptr!=NULL){
		cpy = ptr; 
		ptr = strtok(NULL, " \t"); 
		strcpy(tmp[count], cpy); 
		count++; 
	}

		// To Store to ins
	if(!strcmp(tmp[0], "END")){
		strcpy(ins[i].operation, "END"); 
		strcpy(ins[i].operands, tmp[1]); 
		if(tmp[2]!=NULL)
			strcpy(ins[i].comment, tmp[2]);
	}
	else if(!strcmp(tmp[1], "RESW") || !strcmp(tmp[1], "RESB") || !strcmp(tmp[1], "WORD") || !strcmp(tmp[1], "BYTE")){
		strcpy(ins[i].label, tmp[0]); 
		strcpy(ins[i].operation, tmp[1]); 	
		strcpy(ins[i].operands, tmp[2]); 
		if(tmp[3]!=NULL)
			strcpy(ins[i].comment, tmp[3]); 
	}
	else{
		for(j=0;j<26;j++){
			if(!strcmp(op[j].code, tmp[0])){
				strcpy(ins[i].operation, tmp[0]); 
				if(!strcmp(ins[i].operation, "RSUB")){
					if(tmp[1]!=NULL)
						strcpy(ins[i].comment, tmp[1]); 
					break;	
				}
				if(tmp[1]==NULL)
					break;
				strcpy(ins[i].operands, tmp[1]); 
				if(tmp[2]==NULL)
					break;
				strcpy(ins[i].comment, tmp[2]); 
				break;
			}
			else if(!strcmp(op[j].code, tmp[1])){
				strcpy(ins[i].label, tmp[0]);
				strcpy(ins[i].operation, tmp[1]); 
				if(tmp[2]==NULL)
					break;
				strcpy(ins[i].operands, tmp[2]); 
				if(tmp[3]==NULL)
					break;
				strcpy(ins[i].comment, tmp[3]); 
				break;
			}
		}
	}
	i++; 
	N = i; 
}

void pass1(){
	//	Scanning first time. 
	//		1. To give all instruction Local Address. 
	//		2. To create symbol table. 	

	int i, j, count, len; 
	uint16_t loc;
	char tmp[100], *ptr;  

	count = atoi(ins[start].operands); 
	loc = (count/1000)*16*16*16 + ((count%1000)/100)*16*16 + ((count%100)/10)*16 + count%10; 
	ins[start].loc = loc; 

	count = 0; 
	for(i=0;i<N;i++){
		if(!strcmp(ins[i].operation, "."))
			continue; 

		if(i<=start)
			continue; 

		// Give the loc
		ins[i].loc = loc;
		if(!strcmp(ins[i].operation, "BYTE")){
			strcpy(tmp, ins[i].operands); 
			ptr = strtok(tmp, "'"); 
			if(!strcmp(ptr, "C")){
				ptr = strtok(NULL, "'"); 
				len = strlen(ptr);
				loc += len; 
			}
			else{
				ptr = strtok(NULL, "'"); 
				len = strlen(ptr); 
				loc += len/2; 
			}
		}
		else if(!strcmp(ins[i].operation, "RESB")){
			strcpy(tmp, ins[i].operands); 
			loc += atoi(tmp); 
		}
		else if(!strcmp(ins[i].operation, "RESW")){
			strcpy(tmp, ins[i].operands); 
			loc += atoi(tmp)*3; 	
		}
		else
			loc += 3;

		// Store into symbol table
		if(ins[i].label[0]){
			sym[count].addr = ins[i].loc; 
			strcpy(sym[count].symbol, ins[i].label); 
			count++;
		}
	}
	sN = count; 
}

void pass2(){
	//	Scanning second times. 
	//		1. To complete the object code. 	
	int i, j, count, len;
	char tmp[100], *ptr;
	 
	 

	for(i=0;i<N;i++){
		if(!strcmp(ins[i].operation, "."))
			continue;

		// START, END
		if(!strcmp(ins[i].operation, "END") || !strcmp(ins[i].operation, "START"))
			continue; 
	
		// BYTE, WORD
		if(!strcmp(ins[i].operation, "BYTE")){
			strcpy(tmp, ins[i].operands); 
			ptr = strtok(tmp, "'"); 
			if(!strcmp(ptr, "X")){
				ptr = strtok(NULL, "'"); 
				sscanf(ptr, "%X", &ins[i].obj); 	
			}
			else if(!strcmp(ptr, "C")){
				ptr = strtok(NULL, "'"); 
				len = strlen(ptr); 
				for(j=0;j<len;j++)
					ins[i].obj += (ptr[len-1-j]<<(j*8)); 
			}
			continue; 
		}
		else if(!strcmp(ins[i].operation, "WORD")){
			strcpy(tmp, ins[i].operands); 
			count = atoi(tmp);
			ins[i].obj += count; 
			continue; 
		}

		// RESW, RESB
		if(!strcmp(ins[i].operation, "RESW") || !strcmp(ins[i].operation, "RESB"))
			continue; 	

		// Others
			// get opcode
		for(j=0;j<26;j++){
			if(!strcmp(ins[i].operation, op[j].code)){
				ins[i].obj += (op[j].opcode<<16); 
				break;	
			}
		}
			// get X
		strcpy(tmp, ins[i].operands); 
		ptr = strtok(tmp, ",");
		ptr = strtok(NULL, ",");
		if(ptr!=NULL)
			if(!strcmp(ptr, "X"))
				ins[i].obj += (1<<15); 	
			// get address	
		for(j=0;j<sN;j++){
			if(!strcmp(ins[i].operands, sym[j].symbol)){
				ins[i].obj += sym[j].addr; 	
				break;
			}
		}
	}
}

void ini(){
	memset(ins, 0, sizeof(ins)); 
	memset(sym, 0, sizeof(sym)); 	
}

void assembler(){

	printf("------ PASS 1 ------\n");
	pass1(); 
	showSym();  
	printf("------ PASS 2 ------\n"); 
	pass2();
	showIns();  
}

void toTarget(FILE *fout){
	int i, j, k, space[15], len, a, b; 
	uint16_t front, count, countSpace; 
	char *ptr, tmp[100]; 

	// Header record
	fprintf(fout, "H%-6s%.*s%s%06X\n", ins[start].label, 6-strlen(ins[start].operands), "000000", ins[start].operands, ins[N-1].loc-ins[start].loc); 
//*	
	fprintf(fout, " ^     ^     ^\n"); //*/
	// Text record
	front = ins[start].loc;
	for(i=1;i<N;i+=10){
		// count length
		count = countSpace = 0; 
		space[0] = 2; 
		space[1] = 6; 
		space[2] = 2; 
		countSpace = 3; 
		
		a = i; 
		b = 0;
		 
		for(j=0; j<10;j++){
			if(!strcmp(ins[i+j].operation, "END"))
				break;
			if(!strcmp(ins[i+j].operation, ".")){
				i++; 
				j--; 
				continue; 	
			}
			if(!strcmp(ins[i+j].operation, "RESW") || !strcmp(ins[i+j].operation, "RESB"))
				continue;
			if(!strcmp(ins[i+j].operation, "BYTE")){
				strcpy(tmp, ins[i+j].operands); 
				ptr = strtok(tmp, "'");
				if(!strcmp(ptr, "X")){
					ptr = strtok(NULL, "'"); 
					len = strlen(ptr); 
					space[countSpace] = len; 
					count += len/2; 
				}
				else if(!strcmp(ptr, "C")){
					ptr = strtok(NULL, "'"); 
					len = strlen(ptr); 
					space[countSpace] = len*2; 
					count += len; 
				}
			}
			else {
				count += 3; 
				space[countSpace] = 6; 
			}
			countSpace++; 
		}

		b = i+j;

		// output object code to file
		if(count==0)
			continue; 
		fprintf(fout, "T%06X%02X", front, count); 
		countSpace = 3; 
		for(j=a;j<b;j++){
			if(!strcmp(ins[j].operation, "."))
				continue; 	
			if(!strcmp(ins[j].operation, "RESW") || !strcmp(ins[j].operation, "RESB") || !strcmp(ins[j].operation, "START") || !strcmp(ins[j].operation, "END"))
				continue;
			fprintf(fout, "%0*X", space[countSpace], ins[j].obj) ; 
			countSpace++; 
		}
		fprintf(fout, "\n"); 
//*
		for(j=0;j<countSpace-1;j++){
			for(k=1;k<space[j];k++)
				fprintf(fout, " "); 
			fprintf(fout, "^");	
		}
		fprintf(fout, "\n"); 	
//*/
		front = ins[j].loc; 
	}


	// End record
	i = 0;
	while(1){
		i++; 
		if(!strcmp(ins[i].operation, "START"))
			continue; 
		if(!strcmp(ins[i].operation, "END"))
			continue; 
		if(!strcmp(ins[i].operation, "BYTE"))
			continue; 
		if(!strcmp(ins[i].operation, "WORD"))
			continue; 
		if(!strcmp(ins[i].operation, "RESB"))
			continue; 
		if(!strcmp(ins[i].operation, "RESW"))
			continue; 
		break;
	}
	fprintf(fout, "E%06X\n", ins[i].loc);
//*	
	fprintf(fout, " ^\n");  //*/
}

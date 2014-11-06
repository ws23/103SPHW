#include <stdio.h>
#include <string.h>

int main(int argc, int *argv[]){
	FILE *fin, *fout; 
	char SICCode[1000][100], filename[100]; 
	int line; 

	// If argument error, then exit. 
	if(argc!=2 || argc!=3){
		printf("Argument Error!!\n"); 
		printf("Usage: %s SIC_Code [Object_Code]", argv[0]); 
		return 0; 
	}

	// Get the SIC Code in. 
		// Open Source Code File
	fin = fopen(argv[1], "r"); 
	if(fin==NULL){
		printf("SIC Source Code read Error. \n"); 	
		return 0; 
	}
	
		// Store the Source Code into array. 
	line = 0; 
	while(fgets(SICCode[line++], 100, fin))
		; 

	// To assemble the SIC Code to Object Code. 
	assembler(SICCode, line); 			

	// To store object code to the target file. 
	if(argc==3)
		fout = fopen(argv[2], "w"); 
	else{
		strcat(argv[1], ".out");
		fout = fopen(argv[1], "w"); 
	}
	toTarget(fout); 

	return 0; 	
}

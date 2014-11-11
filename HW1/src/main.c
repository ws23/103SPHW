#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	FILE *fin, *fout; 
	char SICCode[100], filename[100]; 
	int line, i;  
	
	// If argument error, then exit. 
	if(argc!=2 && argc!=3){
		printf("Argument Error!!\n"); 
		printf("Usage: %s SIC_Code [Object_Code]\n", argv[0]); 
		return 0; 
	}

	// Get the SIC Code in. 
		// Open Source Code File
	fin = fopen(argv[1], "r"); 
	if(fin==NULL){
		printf("SIC Source Code read Error. \n"); 	
		return 0; 
	}

	printf("-------\n"); 	
		// Store the Source Code into array. 
	line = 0; 
	while(fgets(SICCode, 100, fin)){
		printf("11111\n"); 
		for(i=0;SICCode[i];i++)
			if(SICCode[i]=='\n' || SICCode[i]=='\r')
				SICCode[i] = 0; 
		printf("22222\n"); 
		addCode(SICCode);
		printf("33333\n"); 
		line++; 
	}
		
	printf("-------\n"); 
	// To assemble the SIC Code to Object Code. 
	assembler(line); 			

	printf("-------\n"); 
	// To store object code to the target file. 
	if(argc==3)
		fout = fopen(argv[2], "w"); 
	else{
		strcat(argv[1], ".out");
		fout = fopen(argv[1], "w"); 
	}
	printf("------\n"); 
	toTarget(fout); 

	return 0; 	
}

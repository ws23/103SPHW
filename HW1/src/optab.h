#include <stdint.h>

typedef struct _optab{
	char code[10]; 
	uint8_t opcode; 
} optab; 

optab op[26] = {
{"ADD", 0x18}, 
{"DIV", 0x24}, 
{"JGT", 0x34}, 
{"LDA", 0x00}, 
{"LDX", 0x04}, 	
{"RD", 0xD8}, 
{"STCH", 0x54}, 
{"STX", 0x10}, 
{"TIX", 0x2C}, 
{"AND", 0x40}, 
{"J", 0x3C}, 
{"JLT", 0x38}, 
{"LDCH", 0x50}, 
{"MUL", 0x20}, 
{"RSUB", 0x4C}, 
{"STL", 0x14}, 
{"SUB", 0x1C}, 
{"WD", 0xDC}, 
{"COMP", 0x28}, 
{"JEQ", 0x30}, 
{"JSUB", 0x48}, 
{"LDL", 0x08}, 
{"OR", 0x44}, 
{"STA", 0x0C}, 
{"STSW", 0x18}, 
{"TD", 0x10}
}; 

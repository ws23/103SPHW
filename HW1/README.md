Programming Assignment #1

Assignment Description

	Write an SIC assembler that reads an SIC assembly program, translates SIC statements into their machine code equivalents, and generates an object file. 
  
Goals

	1. Get familiar with C programming language. 
	2. Learn to use the I/O facilities and library functions provided by standard C. 
	3. Get experience with the system-level programming. 
	4. Get experience with separate compilation, make utility, and C debugger. 
  
Guideline: 

	1. You have to demonstrate your program in person. 
	2. When demonstrating your program, have one of the following formats of the report with you: 
		1) A pdf file onexternal storage tha is accessible with USB interface, or
		2) A paper copy. 
	3. You will get 30% bonus (i.e., your score for this assignment *1.3) if you succeed in demonstrating your program on November 13 between 14:10 and 17:00. 
	4. Your report have to include the following elements: 
		1) A cover page. 
		2) The problem description. 
		3) Highlight of the way you write te program. 
		4) The program listing. 
		5) Test run results. 
		6) Discussion. 
    
--

Programming Assignment #1 Addendum

Assembler Directives

	START, END, BYTE, WORD, RESB, and RESW. 
    
Format of SIC Assembly Program

	1. Each line has 4 fields: 
		1) (Optional) Label (up to 6 characters) 
		2) Operation
		3) (optional) Operands (no blank allowed) 
		4) (optional) COmment
	2. If the first character is a '.', the whole line is a comment. 
	3. There is no blank line in an SIC assembly program source file, except for the last line. 


SIC OPCODE Table

	Mnemonic  Opcode
	  ADD   18
	  DIV   24
	  JGT   34
	  LDA   00
	  LDX   04
	  RD    D8
	  STCH  54
	  STX   10
	  TIX   2C
	  AND   40
	  J     3C
	  JLT   38
	  LDCH  50
	  MUL   20
	  RSUB  4C
	  STL   14
	  SUB   1C
	  WD    DC
	  COMP  28
	  JEQ   30
	  JSUB  48
	  LDL   08
	  OR    44
	  STA   0C
	  STSW  18
	  TD    10

SIC Instruction Format

	opcode: 8 bits
	x: 1 bit
	address: 15 bits
	    
	Mode    Indecation  Target Address
	Direct  x = 0       TA = address
	Indexed x = 1       TA = address + (X)
  
Types of Record in SIC Object Code

	Header record: 
		Col. 1      H
		Col. 2-7    Program name
		Col. 8-13   Stating address of object program (hexadecimal)
		Col. 14-19  Length of object program in bytes (hexadecimal) 
	    
	Text record: 
		Col. 1      T
		Col. 2-7    Starting address for object code in this record (hexadecimal) 
		Col. 8-9    Length of object code in this record in bytes (hexadecimal) 
		Col. 10-69  Object codem represented in hexadecimal (2 columns per byte of object code) 
	    
	End record: 
		Col. 1      E
		Col. 2-7    Address of first executable instruction in object program (hexadecimal) 

Example Source Program

	COPY    START   1000
	FIRST   STL     RETADR
	CLOOP   JSUB    RDREC
			LDA     LENGTH
			COMP    ZERO
			JEQ     ENDFIL
			JSUB    WRREC
			J       CLOOP
	ENDFIL  LDA     EOF
			STA     BUFFER
			LDA     THREE
			STA     LENGTH
			JSUB    WRREC
			LDL     RETADR
			RSUB
	EOF     BYTE    C'EOF'
	THREE   WORD    3
	ZERO    WORD    0
	RETADR  RESW    1
	LENGTH  RESW    1
	BUFFER  RESB    4096
	.
	.
	.
	PDREC   LDX     ZERO
			LDA     ZERO
	RLOOP   TD      INPUT
			JEQ     RLOOP
			RD      INPUT
			COMP    ZERO
			JEQ     EXIT
			STCH    BUFFER,X
			TIX     MAXLEN
			JLT     RLOOP
	EXIT    STX     LENGTH
			RSUB
	INPUT   BYTE    X'F1'
	MAXLEN  WORD    4096
	.
	.
	WRREC   LDX     ZERO
	WLOOP   TD      OUTPUT
			JEQ     WLOOP
			LDCH    BUFFER,X
			WD      OUTPUT
			TIX     LENGTH
			JLT     WLOOP
			RSUB
	OUTPUT  BYTE    X'0'
			END     FIRST

Example Target Program

	HCOPY  00100000107A
	 ^     ^     ^
	T0010001E1410334820390010362810303010154820613C100300102A0C103900102D
	 ^     ^ ^     ^     ^     ^     ^     ^     ^     ^     ^     ^
	T00101E150C10364820610810334C0000454F46000003000000
	 ^     ^ ^     ^     ^     ^     ^     ^     ^
	T0020391E041030001030E0205D30203FD8205D2810303020575490392C205E38203F
	 ^     ^ ^     ^     ^     ^     ^     ^     ^     ^     ^     ^
	T0020571C1010364C0000F1001000041030E02079302064509039DC20792C1036
	 ^     ^ ^     ^     ^ ^     ^     ^     ^     ^     ^     ^    
	T002073073820644C000005
	 ^     ^ ^     ^     ^
	E001000
	 ^

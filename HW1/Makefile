main: src/main.o src/optab.h src/assembler.o
	gcc src/main.o src/optab.h src/assembler.o -o 410121021_58.out -O2 -Wall -Wshadow
	rm -rf src/*.o src/*.gch
main.o: src/main.c
	gcc src/main.c -c

assembler.o: src/assembler.c src/assembler.h
	gcc src/assembler.c src/assembler.h -c

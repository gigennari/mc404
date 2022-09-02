lab2.o: lab2.s
	clang --target=riscv32 -march=rv32g -mabi=ilp32d -mno-relax -g lab2.s -c -o lab2.o

executavel.x: lab2.o 
	ld.lld lab2.o -g -o executavel.x



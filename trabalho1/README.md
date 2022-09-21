# Trabalho 1 - Desmontador 

## Objetivo 

Implementar uma versão simplificada da ferramenta *objdump*. O programa deve reproduzir as saídas das opções 

"-h" - tabela de seções 

"-t" - tabela de símbolos 

"-d" - o código em linguagem de montagem. 

 A entrada e saída de seu programa deverão ser exatamente as mesmas do llvm-objdump-13 para os comandos descritos. 
 
Os comandos acima foram rodados para o lab03 e essas foram as saídas:

```.llvm-objdump-13 -h test.x```

```
Sections:
Idx Name          Size     VMA      Type
  0               00000000 00000000
  1 .text         00000074 000110d4 TEXT
  2 .data         00000004 00012148 DATA
  3 .comment      00000013 00000000
  4 .symtab       00000080 00000000
  5 .shstrtab     00000030 00000000
  6 .strtab       00000033 00000000
```

``` llvm-objdump -t exec1.x```
```
SYMBOL TABLE:
000110d8         .text  00000000 .Lpcrel_hi0
0001113c         .text  00000000 _end
000110e4         .text  00000000 loop
000110fc         .text  00000000 loop_in
00011120         .text  00000000 loop_print
00012148         .data  00000000 n
000110d4         .text  00000000 _start
```

``` llvm-objdump -d test.x```

```
test.x:	file format elf32-littleriscv

Disassembly of section .text:

000110d4 _start:
   110d4: 13 04 01 00                   mv      s0, sp

000110d8 .Lpcrel_hi0:
   110d8: 97 12 00 00                   auipc   t0, 1
   110dc: 83 a2 02 07                   lw      t0, 112(t0)
   110e0: 13 03 10 00                   addi    t1, zero, 1

000110e4 loop:
   110e4: 33 83 62 02                   <unknown>
   110e8: 93 82 f2 ff                   addi    t0, t0, -1
   110ec: e3 9c 02 fe                   bnez    t0, -8
   110f0: 13 0e a0 00                   addi    t3, zero, 10
   110f4: 13 01 c1 ff                   addi    sp, sp, -4
   110f8: 23 20 c1 01                   sw      t3, 0(sp)

000110fc loop_in:
   110fc: 33 65 c3 03                   <unknown>
   11100: 33 43 c3 03                   <unknown>
   11104: 13 05 05 03                   addi    a0, a0, 48
   11108: 13 01 c1 ff                   addi    sp, sp, -4
   1110c: 23 20 a1 00                   sw      a0, 0(sp)
   11110: e3 16 03 fe                   bnez    t1, -20
   11114: 13 05 10 00                   addi    a0, zero, 1
   11118: 13 06 10 00                   addi    a2, zero, 1
   1111c: 93 08 00 04                   addi    a7, zero, 64

00011120 loop_print:
   11120: b3 05 01 00                   add     a1, sp, zero
   11124: 73 00 00 00                   ecall
   11128: 13 01 41 00                   addi    sp, sp, 4
   1112c: e3 1a 24 fe                   bne     s0, sp, -12
   11130: 13 01 41 00                   addi    sp, sp, 4
   11134: b3 05 01 00                   add     a1, sp, zero
   11138: 73 00 00 00                   ecall

0001113c _end:
   1113c: 13 05 00 00                   mv      a0, zero
   11140: 93 08 d0 05                   addi    a7, zero, 93
   11144: 73 00 00 00                   ecall

```

 ## Especificações

 ### Linguagem 
O código deve ser feito em C, em um arquivo chamado ```desmontador.c```

 ### Bibliotecas permitida

unistd.h e fcntl.h




 ## Entrada 
*Executáveis compilados* 

## aída





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
Idx Name              Size     VMA      Type
  0                   00000000 00000000 
  1 .text             00000204 000110b4 TEXT
  2 .comment          00000040 00000000 
  3 .riscv.attributes 00000030 00000000 
  4 .symtab           000000c0 00000000 
  5 .shstrtab         0000003c 00000000 
  6 .strtab           00000071 00000000
```

``` llvm-objdump -t test.x```
```
SYMBOL TABLE:
00000000 l    df *ABS*	00000000 lab03.c
000110b4 g     F .text	00000060 read
00011114 g     F .text	0000004c write
00011160 g     F .text	00000024 dec_to_bin
00011184 g     F .text	00000024 hex_to_bin
000111a8 g     F .text	00000024 bin_to_2complement
000111cc g     F .text	00000024 hex_to_dec
000111f0 g     F .text	00000024 bin_to_hex
00011214 g     F .text	00000020 inverte_endianess
00011234 g     F .text	0000005c main
00011290 g     F .text	00000028 _start
```

``` llvm-objdump -d test.x```

```
test.x:	file format elf32-littleriscv

Disassembly of section .text:

000110b4 <read>:
   110b4: 13 01 01 fe  	addi	sp, sp, -32
   110b8: 23 2e 11 00  	sw	ra, 28(sp)
   110bc: 23 2c 81 00  	sw	s0, 24(sp)
   110c0: 13 04 01 02  	addi	s0, sp, 32
   110c4: 23 2a a4 fe  	sw	a0, -12(s0)
   110c8: 23 28 b4 fe  	sw	a1, -16(s0)
   110cc: 23 26 c4 fe  	sw	a2, -20(s0)
   110d0: 83 26 44 ff  	lw	a3, -12(s0)
   110d4: 03 27 04 ff  	lw	a4, -16(s0)
   110d8: 83 27 c4 fe  	lw	a5, -20(s0)
   110dc: 13 85 06 00  	mv	a0, a3
   110e0: 93 05 07 00  	mv	a1, a4
   110e4: 13 86 07 00  	mv	a2, a5
   110e8: 93 08 f0 03  	addi	a7, zero, 63
   110ec: 73 00 00 00  	ecall	
   110f0: 93 06 05 00  	mv	a3, a0
   110f4: 23 22 d4 fe  	sw	a3, -28(s0)
   110f8: 03 25 44 fe  	lw	a0, -28(s0)
   110fc: 23 24 a4 fe  	sw	a0, -24(s0)
   11100: 03 25 84 fe  	lw	a0, -24(s0)
   11104: 03 24 81 01  	lw	s0, 24(sp)
   11108: 83 20 c1 01  	lw	ra, 28(sp)
   1110c: 13 01 01 02  	addi	sp, sp, 32
   11110: 67 80 00 00  	ret

00011114 <write>:
   11114: 13 01 01 fe  	addi	sp, sp, -32
   11118: 23 2e 11 00  	sw	ra, 28(sp)
   1111c: 23 2c 81 00  	sw	s0, 24(sp)
   11120: 13 04 01 02  	addi	s0, sp, 32
   11124: 23 2a a4 fe  	sw	a0, -12(s0)
   11128: 23 28 b4 fe  	sw	a1, -16(s0)
   1112c: 23 26 c4 fe  	sw	a2, -20(s0)
   11130: 83 26 44 ff  	lw	a3, -12(s0)
   11134: 03 27 04 ff  	lw	a4, -16(s0)
   11138: 83 27 c4 fe  	lw	a5, -20(s0)
   1113c: 13 85 06 00  	mv	a0, a3
   11140: 93 05 07 00  	mv	a1, a4
   11144: 13 86 07 00  	mv	a2, a5
   11148: 93 08 00 04  	addi	a7, zero, 64
   1114c: 73 00 00 00  	ecall	
   11150: 03 24 81 01  	lw	s0, 24(sp)
   11154: 83 20 c1 01  	lw	ra, 28(sp)
   11158: 13 01 01 02  	addi	sp, sp, 32
   1115c: 67 80 00 00  	ret

00011160 <dec_to_bin>:
   11160: 13 01 01 ff  	addi	sp, sp, -16
   11164: 23 26 11 00  	sw	ra, 12(sp)
   11168: 23 24 81 00  	sw	s0, 8(sp)
   1116c: 13 04 01 01  	addi	s0, sp, 16
   11170: 03 25 44 ff  	lw	a0, -12(s0)
   11174: 03 24 81 00  	lw	s0, 8(sp)
   11178: 83 20 c1 00  	lw	ra, 12(sp)
   1117c: 13 01 01 01  	addi	sp, sp, 16
   11180: 67 80 00 00  	ret

00011184 <hex_to_bin>:
   11184: 13 01 01 ff  	addi	sp, sp, -16
   11188: 23 26 11 00  	sw	ra, 12(sp)
   1118c: 23 24 81 00  	sw	s0, 8(sp)
   11190: 13 04 01 01  	addi	s0, sp, 16
   11194: 03 25 44 ff  	lw	a0, -12(s0)
   11198: 03 24 81 00  	lw	s0, 8(sp)
   1119c: 83 20 c1 00  	lw	ra, 12(sp)
   111a0: 13 01 01 01  	addi	sp, sp, 16
   111a4: 67 80 00 00  	ret

000111a8 <bin_to_2complement>:
   111a8: 13 01 01 ff  	addi	sp, sp, -16
   111ac: 23 26 11 00  	sw	ra, 12(sp)
   111b0: 23 24 81 00  	sw	s0, 8(sp)
   111b4: 13 04 01 01  	addi	s0, sp, 16
   111b8: 03 25 44 ff  	lw	a0, -12(s0)
   111bc: 03 24 81 00  	lw	s0, 8(sp)
   111c0: 83 20 c1 00  	lw	ra, 12(sp)
   111c4: 13 01 01 01  	addi	sp, sp, 16
   111c8: 67 80 00 00  	ret

000111cc <hex_to_dec>:
   111cc: 13 01 01 ff  	addi	sp, sp, -16
   111d0: 23 26 11 00  	sw	ra, 12(sp)
   111d4: 23 24 81 00  	sw	s0, 8(sp)
   111d8: 13 04 01 01  	addi	s0, sp, 16
   111dc: 03 25 44 ff  	lw	a0, -12(s0)
   111e0: 03 24 81 00  	lw	s0, 8(sp)
   111e4: 83 20 c1 00  	lw	ra, 12(sp)
   111e8: 13 01 01 01  	addi	sp, sp, 16
   111ec: 67 80 00 00  	ret

000111f0 <bin_to_hex>:
   111f0: 13 01 01 ff  	addi	sp, sp, -16
   111f4: 23 26 11 00  	sw	ra, 12(sp)
   111f8: 23 24 81 00  	sw	s0, 8(sp)
   111fc: 13 04 01 01  	addi	s0, sp, 16
   11200: 03 25 44 ff  	lw	a0, -12(s0)
   11204: 03 24 81 00  	lw	s0, 8(sp)
   11208: 83 20 c1 00  	lw	ra, 12(sp)
   1120c: 13 01 01 01  	addi	sp, sp, 16
   11210: 67 80 00 00  	ret

00011214 <inverte_endianess>:
   11214: 13 01 01 ff  	addi	sp, sp, -16
   11218: 23 26 11 00  	sw	ra, 12(sp)
   1121c: 23 24 81 00  	sw	s0, 8(sp)
   11220: 13 04 01 01  	addi	s0, sp, 16
   11224: 03 24 81 00  	lw	s0, 8(sp)
   11228: 83 20 c1 00  	lw	ra, 12(sp)
   1122c: 13 01 01 01  	addi	sp, sp, 16
   11230: 67 80 00 00  	ret

00011234 <main>:
   11234: 13 01 01 fd  	addi	sp, sp, -48
   11238: 23 26 11 02  	sw	ra, 44(sp)
   1123c: 23 24 81 02  	sw	s0, 40(sp)
   11240: 13 04 01 03  	addi	s0, sp, 48
   11244: 13 05 00 00  	mv	a0, zero
   11248: 23 2c a4 fc  	sw	a0, -40(s0)
   1124c: 23 2a a4 fe  	sw	a0, -12(s0)
   11250: 93 05 04 fe  	addi	a1, s0, -32
   11254: 23 2a b4 fc  	sw	a1, -44(s0)
   11258: 13 06 40 01  	addi	a2, zero, 20
   1125c: 97 00 00 00  	auipc	ra, 0
   11260: e7 80 80 e5  	jalr	-424(ra)
   11264: 83 25 44 fd  	lw	a1, -44(s0)
   11268: 23 2e a4 fc  	sw	a0, -36(s0)
   1126c: 03 26 c4 fd  	lw	a2, -36(s0)
   11270: 13 05 10 00  	addi	a0, zero, 1
   11274: 97 00 00 00  	auipc	ra, 0
   11278: e7 80 00 ea  	jalr	-352(ra)
   1127c: 03 25 84 fd  	lw	a0, -40(s0)
   11280: 03 24 81 02  	lw	s0, 40(sp)
   11284: 83 20 c1 02  	lw	ra, 44(sp)
   11288: 13 01 01 03  	addi	sp, sp, 48
   1128c: 67 80 00 00  	ret

00011290 <_start>:
   11290: 13 01 01 ff  	addi	sp, sp, -16
   11294: 23 26 11 00  	sw	ra, 12(sp)
   11298: 23 24 81 00  	sw	s0, 8(sp)
   1129c: 13 04 01 01  	addi	s0, sp, 16
   112a0: 97 00 00 00  	auipc	ra, 0
   112a4: e7 80 40 f9  	jalr	-108(ra)
   112a8: 03 24 81 00  	lw	s0, 8(sp)
   112ac: 83 20 c1 00  	lw	ra, 12(sp)
   112b0: 13 01 01 01  	addi	sp, sp, 16
   112b4: 67 80 00 00  	ret

```

 ## Especificações

 ### Linguagem 
O código deve ser feito em C, em um arquivo chamado ```desmontador.c```

 ### Bibliotecas permitida

unistd.h e fcntl.h




 ## Entrada 
*Executáveis compilados* 

## aída





#include <stdio.h>
#include <unistd.h>

/*
read
write 
close 
*/

#include <fcntl.h>
/*
open 
*/

#define SIZE_FILE_HEADER 52
#define ADD_E_SHOFF 32
#define SIZE_E_SHOFF 4
#define ADD_E_ESHNUM 48
#define SIZE_E_ESHNUM 2
#define ADD_E_SHSTRNDX 50
#define SIZE_E_SHSTRNDX 2



//"-h" - tabela de seções 


//"-t" - tabela de símbolos 


//"-d" - o código em linguagem de montagem


//função já recebe com endian invertido 
/*
void converte_4_bytes_32bits(char* bytes, char* destino){

//os bytes recebidos estão em hexadecimal, converter pra bin 
for (int i = 0; i < tam; i++){
    //converter cada dígito para binário 
    x = hex[i];
    printf("num é %d\n", x);
    //se for numero
    if(48 <= x <= 57){
        dec_to_bin(hex[i] - 48, 4);
    }
    //se for letra
    if (65 <= x <= 71){
        dec_to_bin(hex[i] - 47, 4);
    }
  }

}

int calcula_immediate(char* bits, int tam){
  int total = 0, potencia = 1;

  for(int i = tam-1; i >=0 ; i--){
    int digito = bits[i] - '0';
    total += digito * potencia; 
    potencia = 2 * potencia;
  }
  return total;
}

void converte_32bits_lm(char* bits){


//0110111 lui 

//0010111 auipc 

//1101111 jal

//1100111 jalr 

//1100011 - B - beq, bne, blt, bge, bltu, bgeu

//0000011 -  I - lb, lh, lw, lbu, lhu

//0100011 - s - sb, sh, sw

//0010011 - I - addi, slti, sltiu, ori, ori, andi, slli
//srli, srai

//0110011 - R - add, sub, sll, slt, sltu, xor, srl, sra, or, and 

//0001111  - I - fence, fence.i

//1110011 - I - ecall, ebreak, cssrrw, cssrrwi, crrwi
//csrrsi, csrrci


}
*/


void inverte_endian(char *str, char *endian_trocado, int tam){
        
    for(int i = tam -1, j=0; i >= 0; i -= 2, j += 2){ 
      endian_trocado[j] = str[i-1];
      endian_trocado[j+1] = str[i]; 
    };
}

int main(int argc, char *argv[])
{
  //argc - numero de comandos totais passado pela linha de comando ao executavel (conta pelos espaços)
  //argv é char array cpntendo cada argumento passado

  /*
  1: ./desmontador -M=no-aliases -d test.x
  2: ./desmontador -t test.x
  3: ./desmontador -h test.x

  */

  //arg 0 é ./desmontador



  //t ou h 
  //econtrar e_shoff = offset em hexadecimal little endian - 0x20	0x28
  //indica que as sections começam a partir desse offset

        
  
  //COPIA SÓ FILE HEADER PARA INFOS INICIAIS 


  int fd = open(argv[argc-1], O_RDONLY);
  unsigned char fileHeader[SIZE_FILE_HEADER];
  read(fd, fileHeader, SIZE_FILE_HEADER); 
  //write(0, fileHeader[0], 1);

  printf("caractere lido %d\n", fileHeader[0]);

  //int e_shoff, e_shnum, e_shstrndx;
  //e_shoff é endereço do começo da section header table.
  
  //for(int i = ++; i < 4; i)
  //e_shnum  number of entries in the section header table.
  //e_shstrndx index of the section header table entry that contains the section names.

  //printf("um byte aí: %d\n", buf[5]);
  
  char c = argv[1][1];
  //encontrar e_shoff


  //encontrar seções 
  if(c == 'h'){
          write(0, "Sections:\n", 11);
          write(0, "Idx Name              Size     VMA      Type\n", 46);
          
          //write(0,"   1 .text             00000204 000110b4 TEXT\n", 47);

  }

  if(c == 't'){
          write(0, "SYMBOL TABLE:\n", 15);
          
  }

  if(c == 'd'){
          
  }



    

  return 0;
}

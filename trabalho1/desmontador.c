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
#define ADD_E_PHNUM 44
#define SIZE_E_PHNUM 2
#define MAX_SIZE 102400



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


/*
exemplo 
vai estar no hex como 44 02 00 00 (244 = 580)

mas vem no array como unsigned char -> 68 2 0 0 -> inverter -> 0 0 2 68 

depois de invertido, 
 0 0 

lê até 16 bytes
*/

int read_value(unsigned char* arr, int offset, int size){
  int v = 0; 
  for(int i = 0; i < size; i++){
    v += arr[offset+i]<<(8*i);
  }
  return v; 
}

void int_dec_to_char_dec(int dec, char* destino) {
    char* ptr = destino, *ptr1 = destino, tmp_char;
    int tmp, n=0;

    do {
        tmp = dec;
        dec /= 10;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz" [(tmp - dec * 10)];
        n+= 1;
    } while ( dec );

    if (tmp < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
}

void print_value(unsigned char* arr, int offset, int size){ 
  for(int i = offset+size-1; i >= offset; i--){
    int x = arr[i];
    printf("%d ", x); 
  }
}

void identify_sections(unsigned char* file, int offset, int num_sections, int num_shtrtab)
{
  //acha endereço da shtrtab
  int a = offset + ((num_shtrtab)* 0x28 + 0x10);
  //printf("endereço do off p shtrtab %d", a); 
  int sh_offset = read_value(file, a, 4);

  //ir no sh_offset - encontrar infos de cada seção

  for(int i = 0; i < num_sections; i++){
    //number

    
    printf("%d              ", i); 
    //name
    //size  
    int aux = offset + (0x28 * i) + 0x14; 
    print_value(file, aux, 8); 
    //write(0, size_arr, 1);
    printf("\n");
  }
  //

}

//

int main(int argc, char *argv[])
{
  //argc - numero de comandos totais passado pela linha de comando ao executavel (conta pelos espaços)
  //argv é char array cpntendo cada argumento passado

  /*
  1: ./desmontador -M=no-aliases -d test.x
  2: ./desmontador -t test.x
  3: ./desmontador -h test.x
  */

  int fd = open(argv[argc-1], O_RDONLY);
  unsigned char file[MAX_SIZE];
  read(fd, file, MAX_SIZE); 

  int e_shoff, e_shnum, e_shstrndx, e_phnum;
  
  //e_shoff é endereço do começo da section header table.
  e_shoff = read_value(file, ADD_E_SHOFF, SIZE_E_SHOFF);
  //printf("valor de e_shoff deu %d\n", e_shoff); 
  //e_shnum  number of entries in the section header table.
  e_shnum = read_value(file, ADD_E_ESHNUM, SIZE_E_ESHNUM);
  //printf("valor de e_shnum deu %d\n", e_shnum); 
  //e_shstrndx index of the section header table entry that contains the section names - número da seção que é a shtrtab
  e_shstrndx = read_value(file, ADD_E_SHSTRNDX, SIZE_E_SHSTRNDX);
  //printf("valor de e_shstrndx deu %d\n", e_shstrndx); 
  //e_phnum number of entries in the program header table
  e_phnum = read_value(file, ADD_E_PHNUM, SIZE_E_PHNUM);
  //printf("%d\n", e_phnum); 
  //lê arquivo inteiro
 
  char c = argv[1][1];

  //encontrar seções 
  if(c == 'h'){
          write(0, "Sections:\n", 11);
          write(0, "Idx Name              Size     VMA      Type\n", 46);
          //sabemos que a shstrtab é começa no offset + 0x2 * e_shstrndx
          
          identify_sections(file, e_shoff, e_shnum, e_shstrndx); 
          //write(0,"   1 .text             00000204 000110b4 TEXT\n", 47);

  }

  if(c == 't'){
          write(0, "SYMBOL TABLE:\n", 15);
          
  }

  if(c == 'd'){
          
  }

  return 0;
}

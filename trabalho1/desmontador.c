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

int numero_digitos(int dec){
  int pot = 1, e = 0, total = 1;
  
  while(total < dec){
    pot *= 2;
    total += pot;
    e += 1;
  }
  return e+1;
}

//devolve o número de dígito do arr em binario
int dec_to_bin(int dec, char *binario){
  //descorbrir quantos digitos vamos usar 
  int n = numero_digitos(dec);
  int i = 0;
  char aux[50];
  while( dec != 1 ){
    int atual = dec % 2; 
    char c =  atual + '0';
    aux[i] = c;
    i++;
    dec = dec / 2; 
  };
  
  if (dec % 2 == 1){
      aux[n-1] = '1';
  }
  else{
      aux[n-1] ='0';
  }
  
  for(int j = n-1, i=0; j >= 0; j--, i++){
    binario[i] = aux[j];
  };
  return n;
}

//devolve o número de dígito do arr em he
int bin_to_hex(char *bin, char *hex, int tam){
  //num de iterações 
  int iter = tam / 4; 
  //residual 
  int res = tam % 4;
  int pos_inv = iter-1;
  int pos_em_hex = 0;
  int n = 0;
  
  //cálculo do resto da frente 
    if(res != 0){
       int total = 0;
        if(res == 3){
            total += (bin[0] - 48) * 4;
            total += (bin[1] - 48) * 2;
            total += (bin[2] - 48) * 1;
        }
        if(res == 2){
            total += (bin[0] - 48) * 2;
            total += (bin[1] - 48) * 1;
        }
        if(res == 1){
            total += (bin[0] - 48) * 1;
        } 
        //escreve no vetor 
        if(total <= 9){
            char c = "0123456789"[total];
            hex[pos_em_hex] = c;
        }
        else{
            char c = total + 'W';
            hex[pos_em_hex] = c;
        }
        pos_em_hex++;
        n++;
    }
    
    int hex_inv[iter];
  
  //de trás pra frente
    for(int i = 0; i < iter; i++){
      int total = 0;
      total += (bin[(tam-1) - (4*i)] - 48) * 1;
      total += (bin[(tam-2) - (4*i)] - 48) * 2;
      total += (bin[(tam-3) - (4*i)] - 48) * 4;
      total += (bin[(tam-4) - (4*i)] - 48) * 8;
      hex_inv[pos_inv] = total;
      pos_inv--;
    }

    for(int j = 0, i=pos_em_hex; j < iter; j++, i++){
        if(hex_inv[j] <= 9){
            int aux = hex_inv[j];
            char c = "0123456789"[aux];
            hex[i] = c;
        }
        else{
            char c = hex_inv[j] + 'W';
            hex[i] = c;
        }
        n++;
    }
    return n;
}


void print_value(unsigned char* arr, int offset, int size){ 
  for(int i = offset+size-1; i >= offset; i--){
    
    if(arr[i] != 0){
      char bin[50];
      int s_bin = dec_to_bin(arr[i], bin);
      char hex[16];
      int s_hex = bin_to_hex(bin, hex, s_bin);
      if(s_hex == 1){
        write(0, "0", 1);
      }
      write(0, hex, s_hex);
    }
    else{
      write(0, "00", 2);
    }

  }
}

int strcompare(char* arr1, char*arr2, int tam){
  for(int i = 0; i < tam; i++){
    if(arr1[i] != arr2[i]){
      return 0;
    }
  }
  return 1;
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
    //printf("%d", i); 
    char num[2]; 
    int_dec_to_char_dec(i, num);
    num[1] = ' ';
    write(0, "  ", 2);
    write(0, num, 2);

    //name
    int name_offset = sh_offset + read_value(file, offset + (i* 0x28), 4); 
    char name[14];
    int s = 0; 
    char c;
    c = file[name_offset]; 

  
    while(c != 0){
      name[s] = file[name_offset+s];
      s++;
      c = file[name_offset+s]; 
    }

    while(s < 14){
      name[s] = ' ';
      s++;
    }
    //printf("%s", name);
    write(0, name, s); 

    //size  
    int aux = offset + (0x28 * i) + 0x14; 
    print_value(file, aux, 4); 
    write(0, " ", 1);
    //write(0, size_arr, 1);
    //

    //VMA - sh_addr - vai p 
    aux = offset + (0x28 * i) + 0x0c; 
    print_value(file, aux, 4); 

    write(0, " ", 1);
    //type
    if(strcompare(name, ".text", 5)){
      write(0, "TEXT", 4);
    }
    if(strcompare(name, ".data", 5)){
      write(0, "DATA", 4);
    }

    //printf("\n");
    write(0, "\n", 1);
  }
  //

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
          write(0, "Idx Name          Size     VMA      Type\n", 42);
          //sabemos que a shstrtab é começa no offset + 0x2 * e_shstrndx
          
          identify_sections(file, e_shoff, e_shnum, e_shstrndx); 
          //write(0,"   1 .text             00000204 000110b4 TEXT\n", 47);

  }

  if(c == 't'){
          write(0, "SYMBOL TABLE:\n", 15);
          int add_symbtab, add_strtab; 

        
          
  }

  if(c == 'd'){
          
  }

  return 0;
}

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


//"-h" - tabela de seções 


//"-t" - tabela de símbolos 


//"-d" - o código em linguagem de montagem


//função já recebe com endian invertido 
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




int main()
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

        
  unsigned char buf[500]; 
          int fd = open(argv[2], O_RDONLY);
          int x = read(fd, buf, 102400); 
        write(0,buf, 200);
        printf(" read leu: %d\n", x);
  
  char c = argv[1][1];


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

void _start(){
  main();
}
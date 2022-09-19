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
    return 0;
}

void _start(){
  main();
}
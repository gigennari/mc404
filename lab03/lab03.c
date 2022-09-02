/* 
** Leitor e Conversor de números inteiros em C**

Entrada:  Um número de 32 bits representado por uma cadeia com até 10 caracteres 
em ASCII, seguida por uma quebra de linha (caracter “\n”)

Caso a cadeia represente um número em base hexadecimal, ela começará com os caracteres “0x”.
Caso contrário, ela começará com um número de 1 a 9 ou com o sinal menos (-), indicando que o número a ser lido é negativo. 
OBS: O sinal de menos (-) não será usado em entradas na representação hexadecimal (p.ex: -0x12 não é uma entrada válida)



Saída: seguintes informações seguidas de quebras de linha:
    O valor na base binária precedido por “0b”. Caso o número seja negativo, você deve tomar o cuidado de mostrar o valor na representação complemento de 2 (como ilustrado no terceiro exemplo abaixo);
    O valor na base decimal supondo que o número de 32 bits está codificado na representação complemento de 2 (Neste caso, se o bit mais significativo for 1, o número é negativo);
    O valor na base hexadecimal precedido por “0x”. Caso o número seja negativo, você deve tomar o cuidado de mostrar o valor na representação complemento de 2 (como ilustrado no terceiro exemplo abaixo)
    O valor na base decimal supondo que o número de 32 bits está codificado na representação sem sinal e que seu endianness foi trocado;
    P.ex. Supondo que o número de 32 bits 0x00545648 foi inserido como entrada, após a troca de endianess, o número passa a ser 0x48565400 e seu valor em decimal é 1213617152.



*/

int read(int __fd, const void *__buf, int __n){
  int bytes;
  __asm__ __volatile__(
    "mv a0, %1           # file descriptor\n"
    "mv a1, %2           # buffer \n"
    "mv a2, %3           # size \n"
    "li a7, 63           # syscall read (63) \n"
    "ecall \n"
    "mv %0, a0"
    : "=r"(bytes)  // Output list
    :"r"(__fd), "r"(__buf), "r"(__n)    // Input list
    : "a0", "a1", "a2", "a7"
  );
  return bytes;
}
 
void write(int __fd, const void *__buf, int __n){
  __asm__ __volatile__(
    "mv a0, %0           # file descriptor\n"
    "mv a1, %1           # buffer \n"
    "mv a2, %2           # size \n"
    "li a7, 64           # syscall write (64) \n"
    "ecall"
    :   // Output list
    :"r"(__fd), "r"(__buf), "r"(__n)    // Input list
    : "a0", "a1", "a2", "a7"
  );
}

//1ª linha:
int dec_to_bin(){

}

int hex_to_bin(){

}

int bin_to_2complement(){
    
}



//2ª linha - converter a partir do binário da primeira linha


int hex_to_dec(){

}

//3ª linha - coneverte a patir do binário da primeira linha para hex
int bin_to_hex(){

}

//4ª linha  - inverter endianess e recalcular hex to dec ou só mandar dec 

void inverte_endianess(){

}

int main()
{
  char str[20];
  int n = read(0, str, 20);
  write(1, str, n);

    
  return 0;
}
 
void _start(){
  main();
}

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
#include <stdlib.h>
#define MAX 50

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

//passa por cada dígito, multiplicando pela potência e somando
//recebe sem Ox.... tam não contabiliza 0x
int hex_to_dec(char *hex, int tam){
    int total = 0, p = 1, x;

    for(int i = tam-1; i >= 0; i--){
      x = hex[i];
      //se for numero
      if(48 <= x && x <= 57){
        total = total + ((x-48) * p);
      }
      //se for letra
      if (65 <= x && x <= 71){
          total =  total + ((x - 55) * p);
      }
      p = p * 16;
    }
    
    return total;
}

int char_dec_to_int_dec(char *dec, int tam){
  int total = 0;
  for(int i = 0; i < tam; i++){
    int digito = dec[i] - '0';
    total *= 10;
    total += digito;
  }
  return total;
}

int int_dec_to_char_dec(int dec, char* destino) {
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
    return n;
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

//divisões sucessivas por 2
int dec_to_bin(int dec, char *binario){
  //descorbrir quantos digitos vamos usar 
  int n = numero_digitos(dec);
  int i = 0;
  char aux[MAX];
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

void bin_to_2complement(char *bin, int tam){
    
    //se houver apenas zeros, basta concatenar 1 na frente 
    int i, x; 
    for(i = tam-1; i >= 0; i--){
      if(bin[i] == '1'){
          break;
      }
    }

    //se i chegar até uma posição antes do início do vetor, só tem zero
    if(i == -1){
      printf("1%s", bin);
    }
    else{
        for (int j = 0; j < i; j++){
            x = bin[j];
            if (x == 49){
                  printf("0");
            }
            else{
                  printf("1");
            }
        }
        
        for (int k = i; k < tam; k++){
            printf("%c", bin[k]);
        }
    }
      
}

//coneverte a patir do binário da primeira linha para hex, tam = tam do binario
//Compactar cada quatro dígitos binários em um único dígito hexa segundo seu valor
int bin_to_hex(char *bin, char *hex, int tam){
  //num de iterações 
  int iter = tam / 4; 
  //residual 
  int res = tam % 4;
  int pos_inv = iter;
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
            char c = total + '7';
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

    for(int j = iter, i=pos_em_hex; j > 0; j--, i++){
        if(hex_inv[j] <= 9){
            int aux = hex_inv[j];
            char c = "0123456789"[aux];
            hex[i] = c;
        }
        else{
            char c = hex_inv[j] + '7';
            hex[i] = c;
        }
        n++;
    }
    return n;
}

int inverte_endian(char *str, char *endian_trocado, int tam){
 
    int tam_endian_trocado;
    char aux[50];
    if(tam < 8){
        for(int i = 0; i < 8-tam; i++){
            aux[i] = '0';
        }
        for(int j = 8-tam, i=0; j < 8; j++, i++){
            aux[j] = str[i+2];
        }
    }
        
    for(int i = tam+1, j=0; i > 0; i -= 2, j += 2){ 
      endian_trocado[j] = aux[i-1];
      endian_trocado[j+1] = aux[i]; 
    }
    return 8;
}

int main()
{
  char str[20];
  int n = read(0, str, 20);
  write(1, str, n);

  int tam_bin, tam_dec, tam_hex, tam_endian_decimal, int_decimal;
  char v_binario[MAX];
  char v_decimal[MAX];
  char v_hex[MAX];
  char v_endian[MAX];

  //identificar se recebemos hex ou dec
  int hex = 0, dec = 0, aux;
  //como hex comeca c 0x, vamos verificar o segundo dígito
  aux = str[1];
  //pela ascii x = 120 e X = 88
  if(aux == 120 || aux ==  88){
    hex = 1;
  }
  else{
    dec = 1;
  }


  //tratamento se for hex
  if(hex){
    int tam = n-3;
    char valor[MAX]; //menos 1 para tirar o )x e \n
    for(int i = 0; i < tam; i ++){
      valor[i] = str[i+2]; //o +2 pular o 0x e copia só os valores importantes
    }

    int_decimal = hex_to_dec(valor, tam); 

    //1ª linha é valor na base binaria
    tam_bin = dec_to_bin(int_decimal, v_binario);

    //2ª linha é valor na base decimal 
    tam_dec = int_dec_to_char_dec(int_decimal, v_decimal);
    
    //3ª linha é valor na base hexadecimal 
    tam_hex = n-1; //tirar só \n
    for (int i = 0; i < tam_hex; i++){
      v_hex[i] = str[i];
    }
 
    //4ª linha é trocar endianess e calcular hex_to_dec
    char endian_trocado[50];
    tam_endian = inverte_endian(str, endian_trocado, tam);
    //passar hex p dec 
    int aux = hex_to_dec(endian_trocado, 8);
    tam_endian_decimal = int_dec_to_char_dec(aux, v_endian);
    
  }

  //tratamento se for dec
  if(dec){
    //identificar se é pos ou neg pelo primeiro caractere '-'
    int aux = str[0];
    int neg = 0, pos = 0;
    //sinal - vale 45 
    if(aux == 45){
      neg = 1;
    }
    else{
      pos = 1;
    }

    //tratamento se for positivo
    if(pos){
      tam_dec = n-1; //-1 p tirar o \n
      
      //2ª linha é o próprio decimal  
      for(int i = 0; i < tam_dec; i ++){
        v_decimal[i] = str[i]; 
      }

      int_decimal = char_dec_to_int_dec(v_decimal, tam_dec);

      //1ª linha é em binario 
      tam_bin = dec_to_bin(int_decimal, v_binario);

      //3ª linha é o decimal em hex/ usar binario da primeira linha p converter mais facil
      tam_hex = bin_to_hex(v_binario, v_hex, tam_bin);

      //4ª linha é endian trocado
      char endian_trocado[50];
      int tam_endian = inverte_endian(v_hex, endian_trocado, tam_hex);
      //passar hex p dec 
      int aux = hex_to_dec(endian_trocado, 8);
      tam_endian_decimal = int_dec_to_char_dec(aux, v_endian);

    }

    //tratamento se for negativo 
    if(neg){
      tam_dec = n-2; //tirar \n e sinal negativo 
      for(int i = 0; i < tam_dec; i ++){
        v_decimal[i] = str[i]; 
      }

      int_decimal = char_dec_to_int_dec(v_decimal, tam_dec);

    }


  }

  //imprimir 
  for(int i = 0; i< tam_bin; i++){
    printf("%c", v_binario[i]);
  }
  printf("\n"); 
  for(int i = 0; i< tam_dec; i++){
    printf("%c", v_decimal[i]);
  }
  printf("\n"); 
  for(int i = 0; i< tam_hex; i++){
    printf("%c", v_hex[i]);
  }
  printf("\n"); 
  for(int i = 0; i< tam_endian_decimal; i++){
    printf("%c", v_endian[i]);
  }
  printf("\n"); 
  return 0;
}
 
void _start(){
  main();
}

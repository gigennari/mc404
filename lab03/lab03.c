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
#define MAX 50;

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

//divisões sucessivas por 2
void dec_to_bin(int dec, int n){
  
  if(dec == 4){
      printf("0");
   }
   if(dec == 3 || dec == 2){
       printf("00");
   }
   if(dec == 1){
       printf("000");
   }
  
  char bin[n];
  int i = 0;
  
  while( dec != 1 ){
    int atual = dec % 2; 
    char c =  atual + '0';
    bin[i] = c;
    i++;
    dec = dec / 2; 
  };
  
  if (dec % 2 == 1){
      bin[n-1] = '1';
  }
  else{
      bin[n-1] ='0';
  }

  for(int j = n; j >= 0; j--){
    printf("%c", bin[j]);
  };
}

//expandir cada dígito hexa em quatro dígitos binários, tam é a qtde de dígitos no numero hex 
void hex_to_bin(char *hex, int tam){
  int x;

  for (int i = 0; i < tam; i++){
    //converter cada dígito para binário 
    x = hex[i];
    //se for numero
    if(48 <= x && x <= 57){
        dec_to_bin(x - 48, 4);
    }
    //se for letra
    if (65 <= x && x <= 71){
        dec_to_bin(x - 47, 4);
    }
  }
  
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

//2ª linha - converter a partir do binário da primeira linha

//passa por cada dígito, multiplicando pela potência e somando
void hex_to_dec(char *hex, int tam){
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
    printf("%d", total);
}

//3ª linha - coneverte a patir do binário da primeira linha para hex
//Compactar cada quatro dígitos binários em um único dígito hexa segundo seu valor
void bin_to_hex(char *bin, int tam){
  //num de iterações 
  int iter = tam / 4; 
  //residual 
  int res = tam % 4;
  int pos = iter;
  
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
        //impressao
        if(total <= 9){
        printf("%d", total);
        }
        else{
            char c = total + '7';
            printf("%c", c);
        }
    }
    
    int hex_inv[iter];
  
  //de trás pra frente
  for(int i = 0; i < iter; i++){
      int total = 0;
      total += (bin[(tam-1) - (4*i)] - 48) * 1;
      total += (bin[(tam-2) - (4*i)] - 48) * 2;
      total += (bin[(tam-3) - (4*i)] - 48) * 4;
      total += (bin[(tam-4) - (4*i)] - 48) * 8;
      

      hex_inv[pos] = total;
      pos--;
  }

  //impressao
  for(int j = iter; j > 0; j--){
    
        if(hex_inv[j] <= 9){
            printf("%d", hex_inv[j]);
        }
        else{
            char c = hex_inv[j] + '7';
            printf("%c", c);
        }
  }
};


int main()
{
  char str[20];
  int n = read(0, str, 20);
  write(1, str, n);

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
    char valor[50]; //menos 1 para tirar o )x e \n
    for(int i = 0; i < tam; i ++){
      valor[i] = str[i+2]; //o +2 pular o 0x e copia só os valores importantes
    }

    //1ª linha é valor na base binaria
    hex_to_bin(valor, tam);

    //2ª linha é valor na base decimal 
    hex_to_dec(valor, tam);

    //3ª linha é valor na base hexacimal 
    printf("%s", str); 

    //4ª linha é trocar endianess e calcular hex_to_dec
    char endian_trocado[50];
    char aux[50];
    if(tam < 8){
        for(int i = 0; i < 8-tam; i++){
            aux[i] = '0';
        }
        for(int j = 8-tam, i=0; j < 8; j++, i++){
            aux[j] = valor[i];
        }
    }
        
    for(int i = tam+1, j=0; i > 0; i -= 2, j += 2){
        
      endian_trocado[j] = aux[i-1];
      endian_trocado[j+1] = aux[i]; 
    }
    
    hex_to_dec(endian_trocado, 8);
  }

  //tratamento se for dec
  if(dec){
    //identificar se é pos ou neg
    int aux = str[0];
    int neg = 0;
    //sinal - vale 45 
    if(aux == 45){
      neg = 1;
    }

    //1ª linha é converter para binário - se neg, em complemento de 2
    char bin[50];

    dec_to_bin(10, bin);

    
    //2ª linha é valor na base decimal - só imprimir 
    printf("%s\n", str);

    //3ª linha é valor hexadecimal 
    printf("Ox");
    //

    //

    //4ª linha é trocar endianess




  }


    
  return 0;
}
 
void _start(){
  main();
}

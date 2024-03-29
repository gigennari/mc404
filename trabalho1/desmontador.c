#include <unistd.h>
#include <fcntl.h>

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


//função já recebe com endian invertido 


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


int int_dec_to_char_dec2(int dec, char* destino) {
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


void calcula_immediate(int x){
  int imm = x & ~0b111111111111; 
  char vimm[5];
  int s = int_dec_to_char_dec2(imm, vimm);
  write(1, vimm, s);
}

void identify_instruction(int x){

int inst =  x & 0b1111111;

  switch (inst) {
    //0110111 lui 
    case 0b0110111:
        write(1, "lui", 3);
        break;
    //0010111 auipc 
    case 0b0010111:

        write(1, "auipc, ", 7);
        //reg

        //imm
        calcula_immediate(x);
        
        
        break;
    
    //1101111 jal
    case 0b1101111:
        
        break;
    //1100111 jalr 
    case 0b1100111:
        
        break;
    //1100011 - B - beq, bne, blt, bge, bltu, bgeu
    case 0b1100011:
        
        break;
    //0000011 -  I - lb, lh, lw, lbu, lhu
    case 0b0000011:
        
        break;
    //0100011 - s - sb, sh, sw
    case 0b0100011:
        
        break;
    //0010011 - I - addi, slti, sltiu, ori, ori, andi, slli, srli, srai
    case 0b0010011:
        
        break;
    //0110011 - R - add, sub, sll, slt, sltu, xor, srl, sra, or, and 
    case 0b0110011:
       
        break;
    //0001111  - I - fence, fence.i
    case 0b0001111:
       
        break;
    //1110011 - I - ecall, ebreak, cssrrw, cssrrwi, crrwi, csrrsi, csrrci
    case 0b1110011:
        
        break;
    
  }




}


//lê até 16 bytes
int read_value(unsigned char* arr, int offset, int size){
  int v = 0; 
  for(int i = 0; i < size; i++){
    v += arr[offset+i]<<(8*i);
  }
  return v; 
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
  char aux[1000];
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
        write(1, "0", 1);
      }
      write(1, hex, s_hex);
    }
    else{
      write(1, "00", 2);
    }

  }
}

void strcopy(char* arr1, char* arr2, int size){
  for(int i = 0; i < size; i++){
    arr1[i] = arr2[i];
  }
  arr1[size+1] = '\0';
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
    write(1, "  ", 2);
    write(1, num, 2);

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
    write(1, name, s); 
    //size  
    int aux = offset + (0x28 * i) + 0x14; 
    print_value(file, aux, 4); 
    write(1, " ", 1);
    //VMA - sh_addr - vai p 
    aux = offset + (0x28 * i) + 0x0c; 
    print_value(file, aux, 4); 

    write(1, " ", 1);
    //type
    if(strcompare(name, ".text", 5)){
      write(1, "TEXT", 4);
    }
    if(strcompare(name, ".data", 5)){
      write(1, "DATA", 4);
    }
    //printf("\n");
    write(1, "\n", 1);
  }
}

void identify_symbols(unsigned char* file, int e_shoff, int e_shnum, int e_shstrndx, int e_phnum){
  write(1, "SYMBOL TABLE:\n", 14);
    int add_symtab, add_strtab, num_symbols; 
    //.symtab - endereços dos símbolos. 
    //.strtab” -  nomes dos símbolos

    //vai até shtrtab encontrar seções
    int a = e_shoff + ((e_shstrndx)* 0x28 + 0x10);
    int sh_offset = read_value(file, a, 4);
    char sections[10][14];
    int sizes[10];

    //ir no sh_offset - encontrar infos de cada seção
      for(int i = 0; i < e_shnum; i++){
        int name_offset = sh_offset + read_value(file, e_shoff + (i* 0x28), 4); 
        char name[14];

        int s = 0; 
        char c = file[name_offset]; 

        while(c != 0){
          name[s] = file[name_offset+s];
          s++;
          c = file[name_offset+s]; 
        }

        strcopy(sections[i], name, s); 
        sizes[i] = s;

        if(strcompare(name, ".symtab", 7)){

          add_symtab = read_value(file, e_shoff + (i* 0x28) + 0x10, 4);
          num_symbols = read_value(file, e_shoff + (i* 0x28) + 0x14, 4); 
          num_symbols = num_symbols / 16;
        }

        if(strcompare(name, ".strtab", 7)){
          add_strtab = read_value(file, e_shoff + (i* 0x28) + 0x10, 4);
        }        
      }

      /*Na seção .symtab, para cada símbolo:
      Os primeiros 4 bytes representam o offset do nome do símbolo na seção “.strtab”.
      Os 4 bytes seguintes representam o endereço do símbolo na memória do programa.
      Os últimos 8 bytes representam outras informações que não nos são úteis neste momento.
      */
    
    for(int i = 1; i < num_symbols; i++){
      int a = add_symtab + (16*i);
      int off_in_strtab = read_value(file, a, 4);
      print_value(file, a + 4, 4);
      write(1, " ", 1);

      //local ou global 
      unsigned char sh_info = file[a+12];
      sh_info = sh_info >>4; 
      if(sh_info == 0){
        write(1, "l ", 2);
      }
      if(sh_info == 1){
         write(1, "g ", 2);
      }


      //seção
      int section = read_value(file, a+14, 2);

      if(section > e_phnum){
        write(1, "*ABS*", 5);
      }
      else{
        write(1, sections[section], sizes[section]);
      }
      write(1, " ", 1);


      //zeross
      print_value(file, a + 8, 4);
      write(1, "  ", 2);
      //nome
      int pos = add_strtab + off_in_strtab;
      char c = file[pos];
      char aux[30];
      int s = 0; 
      
      while(c != 0){
        aux[s] = c;
        pos++;
        c = file[pos];
        s++; 
      }
      write(1, aux, s);
      write(1, "\n", 1);
    }    
}

void print_instruction(unsigned char *file, int offset){
  for(int i = offset; i < offset+4; i++){
    if(file[i] != 0){
      char bin[32];
      int s_bin = dec_to_bin(file[i], bin);
      char hex[16];
      int s_hex = bin_to_hex(bin, hex, s_bin);
      if(s_hex == 1){
        write(1, "0", 1);
      }
      write(1, hex, s_hex);
    }
    else{
      write(1, "00", 2);
    }
     write(1, " ", 1);
  }
}



void disassembly_section(unsigned char* file, int e_shoff, int e_shnum, int e_shstrndx, int e_phnum){
  write(1, "\n", 1);
  write(1, "Disassembly of section .text:\n", 30);
  //econtrar ssection .text   
  //rodar mesmo algoritmo da -t para achar todos os simbolos

  int add_symtab, add_strtab, num_symbols, add_text, size_text, vma_text; 
    //.symtab - endereços dos símbolos. 
    //.strtab” -  nomes dos símbolos

    //vai até shstrtab encontrar seções
    int a = e_shoff + ((e_shstrndx)* 0x28 + 0x10);
    int sh_offset = read_value(file, a, 4);
    char sections[10][14];
    //int sizes[10];

    //ir no sh_offset - encontrar infos de cada seção
      for(int i = 0; i < e_shnum; i++){
        int name_offset = sh_offset + read_value(file, e_shoff + (i* 0x28), 4); 
        char name[14];

        int s = 0; 
        char c = file[name_offset]; 

        while(c != 0){
          name[s] = file[name_offset+s];
          s++;
          c = file[name_offset+s]; 
        }

        strcopy(sections[i], name, s); 
        //sizes[i] = s;

        if(strcompare(name, ".symtab", 7)){

          add_symtab = read_value(file, e_shoff + (i* 0x28) + 0x10, 4);
          num_symbols = read_value(file, e_shoff + (i* 0x28) + 0x14, 4); 
          num_symbols = num_symbols / 16;
        }

        if(strcompare(name, ".strtab", 7)){
          add_strtab = read_value(file, e_shoff + (i* 0x28) + 0x10, 4);
        }     
        if(strcompare(name, ".text", 5)){
          add_text = read_value(file, e_shoff + (i* 0x28) + 0x10, 4);
          size_text = read_value(file, e_shoff + (i* 0x28) + 0x14, 4);
          vma_text = read_value(file, e_shoff + (0x28 * i) + 0x0c, 4);
        }   
      }

      /*Na seção .symtab, para cada símbolo:
      Os primeiros 4 bytes representam o offset do nome do símbolo na seção “.strtab”.
      Os 4 bytes seguintes representam o endereço do símbolo na memória do programa.
      Os últimos 8 bytes representam outras informações que não nos são úteis neste momento.
      */

    char rotulos[100][30];
    int sizes_rotulos[100];
    int add_rotulos[100];

    
    for(int i = 1; i < num_symbols; i++){
      int a = add_symtab + (16*i);

      int off_in_strtab = read_value(file, a, 4);

      add_rotulos[i] = read_value(file, add_symtab + (16*i) + 4, 4);
      
      //nome
      int pos = add_strtab + off_in_strtab;
      char c = file[pos];
      char aux[30];
      int s = 0; 
      
      while(c != 0){
        aux[s] = c;
        pos++;
        c = file[pos];
        s++; 
      }
      strcopy(rotulos[i], aux, s);  
      sizes_rotulos[i] = s;
    }    

  //printar endereço e nome do simbolo


  for(int i = 0; i < size_text/4; i++){ 
    int pos = add_text + 4*i;
    int end1 = vma_text + 4*i; 
    //printf("end1 %d\n", end1); 
    for(int j = 0; j < num_symbols; j++){
      if(end1 == add_rotulos[j]){

         write(1, "\n", 1);
        
        char bin[32];
        int s_bin =  dec_to_bin(end1, bin);
        char hex[16];
        int s_hex = bin_to_hex(bin, hex, s_bin);

        if(s_hex < 8){
          for(int i = 0; i < 8-s_hex; i++){
            write(1, "0", 1);
          }
        }
        write(1, hex, s_hex);

        write(1, " <", 2);
        write(1, rotulos[j], sizes_rotulos[j]);
        write(1, ">:\n", 3);
      }
    }
    

    char bin[32];
    int s_bin =  dec_to_bin(end1, bin);
    char hex[8];
    int s_hex = bin_to_hex(bin, hex, s_bin);
    write(1, hex, s_hex);
    write(1, ": ", 2);
    print_instruction(file, pos);
   

    //int x = read_value(file, pos, 4);
    //identify_instruction(x); 

     write(1, "\n", 1);
  }
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
  //e_shnum  number of entries in the section header table.
  e_shnum = read_value(file, ADD_E_ESHNUM, SIZE_E_ESHNUM);
  //e_shstrndx index of the section header table entry that contains the section names - número da seção que é a shtrtab
  e_shstrndx = read_value(file, ADD_E_SHSTRNDX, SIZE_E_SHSTRNDX);
  //e_phnum number of entries in the program header table
  e_phnum = read_value(file, ADD_E_PHNUM, SIZE_E_PHNUM);
  
  //lê arquivo inteiro
 
  char c = argv[1][1];

  int tam = 0;
  char aux = argv[argc-1][0];
  while(aux != '\0'){
    tam++;
    aux = argv[argc-1][tam];
  }
  
  write(1, "\n", 1);
  write(1, argv[argc-1], tam);
  write(1, ": file format elf32-littleriscv\n", 32);
  write(1, "\n", 1);

  //"-h" - tabela de seções 
  if(c == 'h'){
          write(1, "Sections:\n", 10);
          write(1, "Idx Name          Size     VMA      Type\n", 41);
          identify_sections(file, e_shoff, e_shnum, e_shstrndx); 
          write(1, "\n", 1);
  } 
  //"-t" - tabela de símbolos 
  if(c == 't'){
    identify_symbols(file, e_shoff, e_shnum, e_shstrndx, e_phnum);
  }
  //"-d" - o código em linguagem de montagem
  if(c == 'd'){
    disassembly_section(file, e_shoff, e_shnum, e_shstrndx, e_phnum); 
  }

  return 0;
}

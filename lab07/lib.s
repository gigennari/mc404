.text 
.globl _start

#void puts ( const char * str );
#imprime a string apontada por * str
puts:
    
    li t0, 0 #tamanho da str 
    la a1, a0 # copia a0 em a1 e dps anda no a0 

    li t1, '0' #caractere 0
    lb t2, 0(a0) #pega letra 
    sub t0, t0, t1  #caractere menos o '0'
    addi a0, a0, 1 #anda para próximo byte em a1
      
    li t3, 10
    bge t0, t3, 
    li t3, 0
    blt t0, t3, out
    j imprime

    incrementa:
    addi t0, t0, 1

    imprime:
    la a1, input_adress     # buffer
    li a2,  20         # size
    li a7, 64           # syscall write (64)
    ecall

# char * gets ( char * str );
gets:


#int atoi (const char * str);
atoi:


#char *  itoa ( int value, char * str, int base );
itoa:

# int time();
time:

#void sleep(int ms);
sleep:

#int approx_sqrt(int x, int iterations);
approx_sqrt:

# void imageFilter(char * img, int width, int height, char filter[3][3]);
imageFilter:





# void exit(int code);
exit:





_start:


s 

2

 


.data

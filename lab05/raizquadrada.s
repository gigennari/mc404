#objetivo: calcular a raiz quadrada de 4 números pelo método babilônico 


#entrada: 0400 5337 2240 9166

#saída: 0020 0073 0047 0095 
    

.text 


.globl _start
_start:

read:
    li a0, 0 # file descriptor = 0 (stdin)
    la a1, input_adress #  buffer
    li a2, 20 # size (lendo apenas 1 byte, mas tamanho é variável)
    li a7, 63 # syscall read (63)
    ecall

    li a3, 0
    li a4, 4
    
converte_int:

    #t1 guarda a entrada em int 

    li t1, 0
    li a5, '0'

    lb t0, 0(a1)
    sub t0, t0, a5
    addi t2, zero, 1000
    mul t0, t0, t2 
    add t1, t1, t0

    lb t0, 1(a1)
    sub t0, t0, a5
    addi t2, zero, 100
    mul t0, t0, t2 
    add t1, t1, t0

    lb t0, 2(a1)
    sub t0, t0, a5
    addi t2, zero, 10
    mul t0, t0, t2 
    add t1, t1, t0

    lb t0, 3(a1)
    sub t0, t0, a5
    addi t2, zero, 1
    mul t0, t0, t2 
    add t1, t1, t0

    mv a0, t1
    
    li t1, 0
    li t2, 11
    li t5, 2

    #t6 = k = y/2
    div t6, a0, t5

babilonico:

    #aux t4 = k + y/k   --- a6 = y/k
    div a6, a0, t6
    add t4, a6, t6
    div t6, t4, t5

    addi t1, t1, 1
    blt t1, t2, babilonico


converte_char:
    
    mv a0, t6

    addi t2, zero, 10
    li t1, 0

    #it 1 
    #pega o reto
    rem t1, a0, t2
    add t1, t1, a5
    sb t1, 3(a1)

    #divide por 10
    div a0, a0, t2

     #it doi
    rem t1, a0, t2
    add t1, t1, a5
    sb t1, 2(a1)

    div a0, a0, t2

    #it 3
    rem t1, a0, t2
    add t1, t1, a5
    sb t1, 1(a1)

    div a0, a0, t2

    #it 4
    rem t1, a0, t2
    add t1, t1, a5
    sb t1, 0(a1)

    addi a1, a1, 5

    addi a3, a3, 1
    blt a3, a4, converte_int
  
write:
    li a0, 1            # file descriptor = 1 (stdout)
    la a1, input_adress     # buffer
    li a2,  20         # size
    li a7, 64           # syscall write (64)
    ecall


.data
input_adress: .skip 0x14  # buffer


#aciona motor e direção - 10
.globl set_motor
set_motor:
    li a7, 10
    ecall
    ret

#aciona freio - 11
.globl set_handbreak
set_handbreak:
    li a7, 11
    ecall
    ret
   
# #ler sensores de luminosidade - 12

.globl read_camera
read_camera:
    li a7, 12
    ecall
    ret

#ler sensores - 13 
.globl read_sensor_distance
read_sensor_distance:
    li a7, 13
    ecall
    ret

#pega posicao - 15 
.globl get_position
get_position:
    li a7, 15
    ecall
    ret

#pega rotacao - 16
.globl get_rotation
get_rotation:
    li a7, 16
    ecall  
    ret

#desenhar linha - 19 
.globl display_image
display_image: 
    li a7, 19
    ecall
    ret

# tempo desde que o sistema foi ligado em ms - 20 
.globl get_time
get_time:
    li a7, 20
    ecall
    ret


# #leitura - 17
read:
    li a0, 0 # file descriptor = 0 (stdin)
    li a2, 1 # size (lendo apenas 1 byte, mas tamanho é variável)
    li a7, 17 # syscall read (63)
    ecall
ret

# #escrita - 18 
write:
    li a0, 1            # file descriptor = 1 (stdout)
    li a2, 1           # size
    li a7, 18           # syscall write (64)
    ecall
ret 

.globl filter_1d_image
filter_1d_image:
ret 

.globl puts
puts:
# a0 é uma string - imprimir até chegar em '\0'

li t0, '\0'
mv t1, a0

    imprimir:
        //write 

        addi sp, sp, -4
        mv a1, sp
        lw t2, (t1)
        sw t2, (a1)
        li a0, 1
        li a2, 1
        jal write

        addi t1, t1, 1
        lw t2, (t1)

        addi sp, sp, 4
        bne t2, t0, imprimir

        //imprime '\n'
        li a1, '\0'

        li a0, 1
        li a2, 1
        jal write


ret

.globl gets
gets:

li t0, '\n'
mv t1, a0

    
    gets1:
        //write 

        addi sp, sp, -4
        mv a1, sp

        jal read 
        lw t2, (a1)
        addi sp, sp, 4

        beq t2, t0, fim 
        
        sw t2, (t1)
        addi t1, t1, 1

        j gets1
        fim: 

       
ret 

.globl atoi
atoi:
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
ret 

.globl itoa
itoa:
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
    blt a3, a4, atoi
ret 

.globl sleep
sleep:
ret 

.globl approx_sqrt
approx_sqrt:

# a0 valor de entrada x
# a1 numero de iterações 

    #aux t2 = k + y/k   --- t1= y/k
    li t0, 0

    div t3, a0, t5

    iteracao:
    div t1, a0, t3
    add t2, t1, t3
    div t3, t2, t5

    addi t0, t0, 1
    blt t0, a1, iteracao

#botar retorno raiz quadrada de x em a0 


ret 
.text 
.globl _start

open:
    la a0, input_file    # endereço do caminho para o arquivo
    li a1, 0             # flags (0: rdonly, 1: wronly, 2: rdwr)
    li a2, 0             # modo
    li a7, 1024          # syscall open 
    ecall
    ret

read:
    la a1, input_adress #  buffer
    li a7, 63 # syscall read (63)
    li a2, 262159
    ecall
    ret

out:
    ret

converte_largura:

    #t1 guarda a entrada em int 

    li a5, '0'
    li a6, 10 

    lb t0, 0(a4)
    sub t0, t0, a5  #caractere menos o '0'

    addi a4, a4, 1 #anda para próximo byte em a1
   
    li t3, 10
    bge t0, t3, out
    li t3, 0
    blt t0, t3, out

    mul t1, t1, a6  #se entrar de novo, multiplica o resultado anterior por 10
    add t1, t1, t0
    mv s8, t1
    j converte_largura

converte_altura:

    #t1 guarda a entrada em int 

    li a5, '0'
    li a6, 10 

    lbu t0, 0(a4)
    sub t0, t0, a5  #caractere menos o '0'

    addi a4, a4, 1 #anda para próximo byte em a1
   
    li t3, 10
    bge t0, t3, out
    li t3, 0
    blt t0, t3, out
    mul t1, t1, a6  #se entrar de novo, multiplica o resultado anterior por 10
    add t1, t1, t0
    mv s7, t1 
    j converte_altura


setCanvasSize:
    li a7, 2201 #(número da syscall)
    ecall
    ret

setScaling:
    li a7, 2202 #(número da syscall)
    ecall
    ret

percorre_matriz:


    addi sp, sp, -4
    sw ra, (sp)

    li a0, 0 #largura 
    li a1, 0 #altura 
   
    mv t0, s8
    
    mv t1, s7

    le_linha:

        le_coluna:
            lbu a2, 0(a4)
            jal setPixel
            addi a4, a4, 1
            addi a0, a0, 1
            blt a0, t1, le_coluna

        li a0, 0
        addi a1, a1, 1
        blt a1, t0, le_linha

        lw ra, (sp)
        addi sp, sp, 4
        ret
    
setPixel:
    mv t3, a2
    slli a2, a2, 8
    add a2, a2, t3
    slli a2, a2, 8
    add a2, a2, t3
    slli a2, a2, 8
    addi a2, a2, 255
    
    li a7, 2200 # syscall setGSPixel (2200)
    ecall
    ret


_start:
jal open
jal read
la a4, input_adress
addi a4, a4, 3

li t1, 0
jal converte_largura #s7 
li t1, 0
jal converte_altura #s8 

addi a4, a4, 4

mv a0, s7
mv a1, s8
jal setCanvasSize
jal percorre_matriz



.data
input_adress: .skip 262159   # buffer
input_file: .asciz "imagem.pgm"

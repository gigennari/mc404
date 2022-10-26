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
    li a0, 0 #largura contador i
    li a1, 0 #altura  contador j
    mv t0, s7 #altura  total
    mv t1, s8 #largura total

    le_linha:
        le_coluna:
            li t5, 0
            beq a0, t5, borda
            beq a1, t5, borda

            addi t5, t0, -1
            beq a1, t5, borda
            addi t5, t1, -1
            beq a0, t5, borda

            li s0, 0 #k
            li s1, 0 #q
            li s2, 3
            li a2, 0

           somat_externo:
                somat_interno:

                    li a5, 0
                    add a5, a5, s0 #soma k
                    addi a5, a5, -1 #soma -1

                    #localizar no buffer 
                    # largura * [k-1]
                    mul a5, a5, t1
                    add a5, a5, s1
                    addi a5, a5, -1

                    #anda no buffer 
                    add a4, a4, a5

                    #carrega info 
                    lbu a3, 0(a4)

                    #volta no buffer 
                    sub a4, a4, a5
                    
                    li t2, -1
                    li t5, 1

                    beq s1, t5, verifica
                    j multiplica

                    verifica:
                    beq s0, t5, altera_fator
                    j multiplica

                    altera_fator:
                    li t2, 8

                    multiplica:
                    mul a3, a3, t2
                    add a2, a2, a3

                    addi s1, s1, 1
                    blt s1, s2, somat_interno
                
                li s1, 0
                addi s0, s0, 1
                blt s0, s2, somat_externo
            
                
                j set
                
            borda: 
                li a2, 0

            set:
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
    #a0 contém coordenada x, a1 coordenada y e a2 cor do pixel
    
    li t2, 1
    blt a2, t2, zera
    li t2, 255
    bge a2, t2, maximiza
    j imprime

    zera: 
    li a2, 0
    j imprime

    maximiza:
    li a2, 255

    imprime:
    mv a3, a2
    slli a3, a3, 8
    add a3, a3, a2
    slli a3, a3, 8
    add a3, a3, a2
    slli a3, a3, 8
    addi a3, a3, 255
    mv a2, a3
    
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

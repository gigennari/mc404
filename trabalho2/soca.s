.section .bss
.align 4
isr_stack: # Final da pilha das ISRs
.skip 1024 # Aloca 1024 bytes para a pilha
isr_stack_end: # Base da pilha das ISRs

.text
.align 4

#SYSCALLS 

#aciona freio - 11
Syscall_set_handbreak:
#a0 =1 aciona freio de mão
   li t0, 0xffff0300
   sb a0, 34(t0)
   ret

#aciona motor e direção - 10
Syscall_set_motor:
#a0: -1 p trás, 0 ou 1 p frente
#a1: -127 até 127 - neg é esq, pos dir
   
    li t0, 0xffff0300

    li t1, 128
    bge a1, t1, invalido  

    li t1, -127
    blt a1, t1, invalido 

    li t1, 2
    bge a0, t1, invalido  

    li t1, -1 
    blt a0, t1, invalido
    
    sucesso:    
    sb a0, 33(t0)
    sb a1, 32(t0)
    li a0, 0
    ret

    invalido: 
    li a0, -1
    ret


# #ler sensores de luminosidade - 12
# Sycall_read_sensors:
# #a0 = endereço de um vetor de 256 elementos q armazena nos lidos 
#     li t0, 0xffff0300
#     li t1, 1
#     sb t1, 1(t0)
#     li t2, 0

#     loop_leitura:
#         lb t1, 1(t0) #t1 vira 0 quando conclui
#         bne t1, t2, loop_leitura
#         beq t1, t2, leitura_concluida

#     leitura_concluida:
#         sb a0, 36(t0)
    
#     ret  

# ler sensores de luminosidade - 12
Sycall_read_sensors:
#a0 = endereço de um vetor de 256 elementos q armazena nos lidos 
    li t0, 0xffff0300
    li t1, 0xffff0300
    addi t1, t1, 36
    li t2, 0
    li t3, 256

    li t4, 1
    sb t4, 1(t0)
    busy_waiting:
        lb t4, 1(t0)
        bne t4, zero, busy_waiting
    
    loop_leitura:
    lb t5, 0(t1)
    sb t5, 0(a0)
    addi a0, a0, 1
    addi t1, t1, 1
    addi t2, t2, 1
    blt t2, t3, loop_leitura
    
    addi a0, a0, -256
    ret  

#ler sensores - 13 
Syscall_read_sensor_distance:
    li t0, 0xffff0300

    li t1, 1
    sb t1, 2(t0)

    li t2, 0

    busy_waiting2:
        lb t1, 2(t0)
        bne t1, t2, busy_waiting2

    lw a0, 28(t0)
    ret  

#pega posicao - 15 
Syscall_get_position:
#a0 end da var q armazena x 
#a1 end da var q armazena y 
#a2 end da var q armazena z 
    li t0, 0xffff0300
    li t1, 1
    sb t1, 0(t0)
    li t2, 0

    busy_waiting3:
        lb t1, 0(t0)
        bne t1, t2, busy_waiting3

    lw t3, 16(t0)
    sw t3, (a0)
 
    lw t3, 20(t0)
    sw t3, (a1)

    lw t3, 24(t0)
    sw t3, (a2)

    ret  

#pega rotacao - 16
Syscall_get_rotation:
#a0 end da var q armazena x 
#a1 end da var q armazena y 
#a2 end da var q armazena z 
    li t0, 0xffff0300

    li t1, 1
    sb t1, 0(t0)

    li t2, 0

    busy_waiting4:
        lb t1, 0(t0)
        bne t1, t2, busy_waiting4
       
        lw t3, 4(t0)
        sw t3, (a0)

        lw t3, 8(t0)
        sw t3, (a1)

        lw t3, 12(t0)
        sw t3, (a2)
    ret  

# #leitura - 17
Syscall_read:
# #a0 file descriptor  
# #a1 buffer 
# #a2 size 
# a0 é 1
# a1 buffer
# a2 size 

           # file descriptor = 1 (stdout) 
    li t2, 0xffff0500
    li t1, 0
    mv t3, a1
    li t4, 1    

    read:

        sb t4, 2(t2) 

        busy_waiting6:
            lb t0, 2(t2)
            bne t0, zero, busy_waiting6

        lb t0, 3(t2)
        sb t0, 0(t3)
       

        addi t1, t1, 1
        addi t3, t3, 1

        bne t1, a2, read


    ret

#escrita 18 
Syscall_write:
# a0 é 1
# a1 buffer
# a2 size 

           # file descriptor = 1 (stdout) 
    li t2, 0xffff0500
    li t1, 0
    mv t3, a1
    li t4, 1    

    write:
        lb t0, 0(t3)
        sb t0, 1(t2)
        sb t4, 0(t2)

        busy_waiting5:
            lb t5, 0(t2)
            bne t5, zero, busy_waiting5


        addi t1, t1, 1
        addi t3, t3, 1

        bne t1, a2, write


    ret


#desenhar linha - 19 
Syscall_draw_line: 
    li t0, 0xffff0300
    sb a0, 24(t0)
    ret 

# tempo desde que o sistema foi ligado em ms - 20 
# Syscall_get_systime:
#   ret



biblioteca_controle:
    ###### Tratador de interrupções e syscalls ######

    # <= Implemente o tratamento da sua syscall aqui 
    csrrw sp, mscratch, sp
    addi sp, sp, -32
    sw ra, (sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    sw t0, 12(sp)
    sw t1, 16(sp)
    sw t2, 20(sp)

    li t0, 0xffff0300
    
    li t1, 10
    beq a7, t1, chama_Syscall_set_motor

    li t1, 11
    beq a7, t1, chama_Syscall_set_handbreak   
 
    li t1, 12
    beq a7, t1, chama_Sycall_read_sensors 

    li t1, 13
    beq a7, t1, chama_Syscall_read_sensor_distance
    
    li t1, 15
    beq a7, t1, chama_Syscall_get_position

    li t1, 16
    beq a7, t1, chama_Syscall_get_rotation

    li t1, 17
    beq a7, t1, Syscall_read
    j continua

    li t1, 18
    beq a7, t1, Syscall_write
    j continua

    li t1, 19
    beq a7, t1, chama_Syscall_draw_line

    chama_Syscall_set_motor:
    jal Syscall_set_motor
    jal continua

    chama_Syscall_set_handbreak:
    jal Syscall_set_handbreak
    jal continua    

    chama_Sycall_read_sensors:
    jal Sycall_read_sensors
    jal continua

    chama_Syscall_read_sensor_distance:
    jal Syscall_read_sensor_distance
    jal continua

    chama_Syscall_get_position:
    jal Syscall_get_position 
    jal continua

    chama_Syscall_get_rotation:
    jal Syscall_get_rotation
    jal continua

    chama_Syscall_draw_line:
    jal Syscall_draw_line
    jal continua

    continua: 
    lw ra, (sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    lw t0, 12(sp)
    lw t1, 16(sp)
    lw t2, 20(sp)
    addi sp, sp, 32

    csrrw sp, mscratch, sp

    csrr t0, mepc  # carrega endereço de retorno (endereço 
                    # da instrução que invocou a syscall)

    addi t0, t0, 4 # soma 4 no endereço de retorno (para retornar após a ecall) 
    csrw mepc, t0  # armazena endereço de retorno de volta no mepc
    mret           # Recuperar o restante do contexto (pc <- mepc)



.globl _start
_start:

  la t0, biblioteca_controle  # Carregar o endereço da rotina que tratará as interrupções
  csrw mtvec, t0      # (e syscalls) em no registrador MTVEC para configurar
                      # o vetor de interrupções.

# Escreva aqui o código para mudar para modo de usuário e chamar a função 
# user_main (definida em outro arquivo). Lembre-se de inicializar a 
# pilha do usuário para que seu programa possa utilizá-la.

    #HABILITAR INTERRUPÇÕES 

    #mstatus.MIE (bit 3) vira 1 
    csrr t1, mstatus # Seta o bit 3 (MIE)
    ori t1, t1, 0x8 # do registrador mstatus
    csrw mstatus, t1

    csrr t1, mie # Seta o bit 11 (MEIE)
    li t2, 0x800 # do registrador mie
    or t1, t1, t2
    csrw mie, t1


    #CONFIGURAR MSCRATCH PARA APONTAR PILHA DA ISR 
    #Fazer mscratch apontar para uma posição de memória que possa ser usada pela ISR
    la t0, isr_stack_end # t0 <= base da pilha
    csrw mscratch, t0 # mscratch <= t0

    li sp, 0x07FFFFFC    

    csrr t1, mstatus # Update the mstatus.MPP
    li t2, ~0x1800 # field (bits 11 and 12)
    and t1, t1, t2 # with value 00 (U-mode)
    csrw mstatus, t1
    la t0, main # Loads the user software
    csrw mepc, t0 # entry point into mepc
    mret
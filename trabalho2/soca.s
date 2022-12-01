.section .bss
.align 4
isr_stack: # Final da pilha das ISRs
.skip 1024 # Aloca 1024 bytes para a pilha
isr_stack_end: # Base da pilha das ISRs

.text
.align 4


biblioteca_controle:
    ###### Tratador de interrupções e syscalls ######

    # <= Implemente o tratamento da sua syscall aqui 
    csrrw sp, mscratch, sp
    addi sp, sp, -4
    sw ra, (sp)
    # addi sp, sp, -64
    
    # sw a0, 0(sp)
    # sw a1, 4(sp)
    # sw a2, 8(sp)
    # sw a3, 12(sp)
    # sw a4, 16(sp)
    # sw a5, 20(sp)
    # sw a6, 24(sp)
    # sw a7, 28(sp)
    # sw t0, 32(sp)
    # sw t1, 36(sp)
    # sw t2, 40(sp)
    # sw t3, 44(sp)
    # sw t4, 48(sp)
    # sw t5, 52(sp)
    # sw t6, 56(sp)

    li t0, 0xffff0300
    
    li t1, 10
    beq a7, t1, chama_Syscall_set_motor
    j continua

    chama_Syscall_set_motor:
    jal Syscall_set_motor
    j continua

    li t1, 11
    beq a7, t1, chama_Syscall_set_handbreak

    chama_Syscall_set_handbreak:
    jal Syscall_set_handbreak
    j continua
 
    li t1, 12
    beq a7, t1, chama_Sycall_read_sensors
    chama_Sycall_read_sensors:
    jal Sycall_read_sensors
    j continua

    li t1, 13
    beq a7, t1, chama_Syscall_read_sensor_distance
    chama_Syscall_read_sensor_distance:
    jal Syscall_read_sensor_distance
    j continua

    li t1, 15
    beq a7, t1, chama_Syscall_get_position
    chama_Syscall_get_position:
    jal Syscall_get_position 
    j continua

    li t1, 16
    beq a7, t1, chama_Syscall_get_rotation
    chama_Syscall_get_rotation:
    jal Syscall_get_rotation
    j continua

    # li t1, 17
    # beq a7, t1, Syscall_read
    # j continua

    # li t1, 18
    # beq a7, t1, Syscall_write
    # j continua

    li t1, 19
    beq a7, t1, chama_Syscall_draw_line

    chama_Syscall_draw_line:
    jal Syscall_draw_line
    j continua

    continua: 
    lw ra, (sp)
    addi sp, sp, 4
    # lw a0, 0(sp)
    # lw a1, 4(sp)
    # lw a2, 8(sp)
    # lw a3, 12(sp)
    # lw a4, 16(sp)
    # lw a5, 20(sp)
    # lw a6, 24(sp)
    # lw a7, 28(sp)
    # lw t0, 32(sp)
    # lw t1, 36(sp)
    # lw t2, 40(sp)
    # lw t3, 44(sp)
    # lw t4, 48(sp)
    # lw t5, 52(sp)
    # lw t6, 56(sp)
    # addi sp, sp, 64
    csrrw sp, mscratch, sp

    csrr t0, mepc  # carrega endereço de retorno (endereço 
                    # da instrução que invocou a syscall)

    addi t0, t0, 4 # soma 4 no endereço de retorno (para retornar após a ecall) 
    csrw mepc, t0  # armazena endereço de retorno de volta no mepc
    mret           # Recuperar o restante do contexto (pc <- mepc)


#SYSCALLS 

Syscall_set_motor:
#a0: -1 p trás, 0 ou 1 p frente
#a1: -127 até 127 - neg é esq, pos dir
   li t0, 0xffff0300
   sb t1, 32(t0) 
   sb t2, 33(t0)

    li t3, 127
    bge a1, t3, falha 

    li t3, -127
    blt a1, t3, falha 

    li t3, -1 
    bne a0, t3, testa_prox
 
 
   sucesso:
    li a0, 0

    retorno:
    li a0, 0
    ret 

#aciona freio - 11
Syscall_set_handbreak:
#a0 =0 aciona freio de mão
   li t0, 0xffff0300
   sb a0, 34(t0)
   ret

#ler sensores de luminosidade - 12
Sycall_read_sensors:
#a0 = endereço de um vetor de 256 elementos q armazena nos lidos 
   li t0, 0xffff0300

    li t1, 1
    sb t1, 0(t0)

    li t2, 0

    loop_leitura:
        sb t1, 1(t0)
        bne t1, t2, loop_leitura
        beq t1, t2, leitura_concluida

    leitura_concluida:
        lb a0, 24(t0)
    
    ret  

#ler sensores - 13 
Syscall_read_sensor_distance:
    li t0, 0xffff0300

    li t1, 1
    sb t1, 0(t0)

    li t2, 0

    loop_leitura1:
        sb t1, 2(t0)
        bne t1, t2, loop_leitura
        beq t1, t2, leitura_concluida

    leitura_concluida1:
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

    loop_leitura2:
        sb t1, 0(t0)
        bne t1, t2, loop_leitura
        beq t1, t2, leitura_concluida

    leitura_concluida2:
    lw a0, 16(t0)
    lw a1, 20(t0)
    lw a2, 24(t0)
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

    loop_leitura3:
        sb t1, 0(t0)
        bne t1, t2, loop_leitura
        beq t1, t2, leitura_concluida

    leitura_concluida3:

    lw a0, 4(t0)
    lw a1, 8(t0)
    lw a2, 12(t0)
    ret  

# #leitura - 63
# Syscall_read:
# #a0 file descriptor  
# #a1 buffer 
# #a2 size 
#     li t0, 0xffff0300
#     li a0, 0
#     lb a1, 8(t0)
#     lb a2, 12(t0)
#     ret 

# #escrita 18 
# Syscall_write:
#     li a0, 1            # file descriptor = 1 (stdout)
#     la a1, input_adress     # buffer
#     li a2,  20 


#desenhar linha - 19 
Syscall_draw_line: 
    li t0, 0xffff0300
    lb a0, 24(t0)
    ret 


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
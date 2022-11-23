.section .bss
.align 4
isr_stack: # Final da pilha das ISRs
.skip 1024 # Aloca 1024 bytes para a pilha
isr_stack_end: # Base da pilha das ISRs
user_stack:
.skip 1024
user_stack_end:

.text
.align 4

int_handler:
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

    li t0, 0xffff0100
    li t1, 10
    beq a7, t1, chama_Syscall_set_engine_and_steering

    li t1, 11
    beq a7, t1, chama_Syscall_set_handbreak

    li t1, 12
    beq a7, t1, chama_Sycall_read_sensors

    li t1, 15
    beq a7, t1, chama_Syscall_get_position

    chama_Syscall_set_engine_and_steering:
    jal Syscall_set_engine_and_steering
    j continua

    chama_Syscall_set_handbreak:
    jal Syscall_set_handbreak
    j continua 

    chama_Sycall_read_sensors:
    jal Sycall_read_sensors
    j continua 

    chama_Syscall_get_position:
    jal Syscall_get_position
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
  
#aciona motor 
Syscall_set_engine_and_steering:
#a0: -1 p trás, 0 ou 1 p frente
#a1: -127 até 127 - neg é esq, pos dir
   li t0, 0xffff0100
    sb a0, 33(t0)
    sb a1, 32(t0) 
    ret 

#aciona freio 
Syscall_set_handbreak:
#a0 =0 aciona freio de mão
   li t0, 0xffff0100
    sb a0, 34(t0) 
    ret 

#ler sensores
Sycall_read_sensors:
#a0 = endereço de um vetor de 256 elementos q armazena nos lidos 
   li t0, 0xffff0100
    li a7, 12  
    ret 

#pega posicao  
Syscall_get_position:
#a0 end da var q armazena x 
#a1 end da var q armazena x 
#a2 end da var q armazena x 
   li t0, 0xffff0100
    li a7, 15 
    ret   


.globl _start
_start:

  la t0, int_handler  # Carregar o endereço da rotina que tratará as interrupções
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

    la sp, user_stack_end   

    csrr t1, mstatus # Update the mstatus.MPP
    li t2, ~0x1800 # field (bits 11 and 12)
    and t1, t1, t2 # with value 00 (U-mode)
    csrw mstatus, t1
    la t0, user_main # Loads the user software
    csrw mepc, t0 # entry point into mepc
    mret

.globl logica_controle
logica_controle:
  # implemente aqui sua lógica de controle, utilizando apenas as 
  # syscalls definidas.

    li t1, 0
    li t2, 2800

    li a0, 1
    li a1, -60
    li a7, 10
    ecall 

    anda1:
    addi t1, t1, 1
    bne t1, t2, anda1

    li a0, 1
    li a1, 0
    li a7, 10
    ecall 

    li t1, 0
    li t2, 6000
    
    anda3:
    addi t1, t1, 1
    bne t1, t2, anda3

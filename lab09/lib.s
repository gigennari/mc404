.section .bss
.align 4
isr_stack: # Final da pilha das ISRs
.skip 1024 # Aloca 1024 bytes para a pilha
isr_stack_end: # Base da pilha das ISRs

.text  
.globl _start
_start:

#REGISTRAR ISR 
la t0, tratador_interrupcao # Grava o endereço da ISR principal
csrw mtvec, t0 # no registrador mtvec

#CONFIGURAR MSCRATCH PARA APONTAR PILHA DA ISR 
#Fazer mscratch apontar para uma posição de memória que possa ser usada pela ISR
la t0, isr_stack_end # t0 <= base da pilha
csrw mscratch, t0 # mscratch <= t0

#CONFIGURAR PERIFÉRICOS
# li s0, 0xFFFF0100 #começo gpt
# li s1, 0xFFFF0300 #começo midi  

#HABILITAR INTERRUPÇÕES 

#mstatus.MIE (bit 3) vira 1 
csrr t1, mstatus # Seta o bit 3 (MIE)
ori t1, t1, 0x8 # do registrador mstatus
csrw mstatus, t1


csrr t1, mie # Seta o bit 11 (MEIE)
li t2, 0x800 # do registrador mie
or t1, t1, t2
csrw mie, t1

li t0, 0xffff0100
li t1, 100
sw t1, 8(t0)

jal main 

li a0, 0
li a7, 93 
ecall

play_note:
    li t2, 0xffff0300

    #a0 ch
    #a1 inst - id do instrumento musical
    #a2 note
    #a3 vel
    #a4 dur 
    #s0 contém midi
    #toca quando a0 é diferente de -1, se for -1 n toca 
    sb a0, 0(t2)
    sh a1, 2(t2)
    sb a2, 4(t2)
    sb a3, 5(t2)
    sh a4, 6(t2)
    ret 


tratador_interrupcao:  

    #salva o contexto
    csrrw sp, mscratch, sp
    addi sp, sp, -64
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    sw a3, 12(sp)
    sw a4, 16(sp)
    sw a5, 20(sp)
    sw a6, 24(sp)
    sw a7, 28(sp)
    sw t0, 32(sp)
    sw t1, 36(sp)
    sw t2, 40(sp)
    sw t3, 44(sp)
    sw t4, 48(sp)
    sw t5, 52(sp)
    sw t6, 56(sp)

    #trata interrupção 
    li t1, 0xffff0100
    li t0, 100
    sw t0, 8(t1)

    la t3, _system_time
    lw t2, (t3)
    addi t2, t2, 100
    sw t2, (t3) 

    #recupera contexto 
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    lw a3, 12(sp)
    lw a4, 16(sp)
    lw a5, 20(sp)
    lw a6, 24(sp)
    lw a7, 28(sp)
    lw t0, 32(sp)
    lw t1, 36(sp)
    lw t2, 40(sp)
    lw t3, 44(sp)
    lw t4, 48(sp)
    lw t5, 52(sp)
    lw t6, 56(sp)
    addi sp, sp, 64
    csrrw sp, mscratch, sp
    mret 

.data
.globl play_note
.globl _system_time 
_system_time: .word 0
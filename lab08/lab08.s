.text 
.globl _start


sleep:
    addi sp, sp, -4 
    sw ra, (sp)
    addi sp, sp, -4 
    sw a0, (sp)
    call time 
    mv t0, a0

    loop_time:
        call time 
        mv t1, a0
        sub t2, t1, t0
        lw t3, 0(sp)
        bge t3, t2, loop_time 

    addi sp, sp, -4 
    lw ra, (sp)

time:
  addi sp, sp, -12
  mv a0, sp
#   la a0, buffer_timeval
  addi sp, sp, -8
  mv a1, sp
#   la a1, buffer_timezone
  addi sp, sp, -4
  mv a0, sp
  addi sp, sp, -4
  mv a1, sp

  li a7, 169 # chamada de sistema gettimeofday
  ecall

  mv a1, sp
  addi sp, sp, 4
  mv a0, sp
  addi sp, sp, 4
 lw t1, 0(a0) # tempo em segundos
  lw t2, 8(a0) # fração do tempo em microssegundos
  li t3, 1000
  mul t1, t1, t3
  div t2, t2, t3
  add a0, t2, t1
 ret 




_start:
li s0, 0xFFFF0100

li s1, 73
li s2, 1

# muda dir voltante 

li t1, 0
li t2, 13000 

li a0, -60
sb a0, 32(s0)

anda:
li a0, 1
sb a0, 33(s0)
addi t1, t1, 1
bne t1, t2, anda

continua:
li a0, 0
sb a0, 32(s0)
li t1, 0
j anda

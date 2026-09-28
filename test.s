    .section .text
    .globl _start
_start:
    addi x1, x0, 5      # x1 = 5
    addi x2, x0, 7      # x2 = 7
    add  x3, x1, x2     # x3 = x1 + x2 = 12
    addi x4, x3, -20    # x4 = 12 - 20 = -8 (tests sign extension)
    sub  x5, x3, x1     # x5 = 12 - 5 = 7
    ecall               # stop

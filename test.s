    .section .text
    .globl _start
_start:
    addi x1, x0, 5      # x1 = 5
    addi x2, x0, 7      # x2 = 7
    add  x3, x1, x2     # x3 = x1 + x2 = 12
    addi x4, x3, -20    # x4 = 12 - 20 = -8 (tests sign extension)
    sub  x5, x3, x1     # x5 = 12 - 5 = 7
    and  x6, x1, x2     # x6 = 5 AND 7 = 5
    or   x7, x1, x2     # x7 = 5 OR 7 = 7
    xor  x8, x1, x2     # x8 = 5 XOR 7 = 2
    andi x9,  x2, 3     # x9  = 7 AND 3  = 3
    ori  x10, x1, 8     # x10 = 5 OR 8   = 13
    xori x11, x2, -1    # x11 = 7 XOR -1 = -8
    ecall               # stop

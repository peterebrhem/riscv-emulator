

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MEM_SIZE (1024 * 1024)  

uint8_t  mem[MEM_SIZE];   
uint32_t regs[32];       
uint32_t pc = 0;          


static void illegal(uint32_t inst) {
    fprintf(stderr, "Unknown instruction 0x%08x at pc 0x%08x\n", inst, pc);
    exit(1);
}


static uint32_t mem_read32(uint32_t addr) {
    if (addr > MEM_SIZE - 4) {
        fprintf(stderr, "Memory read out of bounds: 0x%08x\n", addr);
        exit(1);
    }
    return mem[addr]
         | (mem[addr + 1] << 8)
         | (mem[addr + 2] << 16)
         | ((uint32_t)mem[addr + 3] << 24);
}

static void load_program(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) { perror(path); exit(1); }
    size_t n = fread(mem, 1, MEM_SIZE, f);
    fclose(f);
    printf("Loaded %zu bytes from %s\n", n, path);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s program.bin\n", argv[0]);
        return 1;
    }
    load_program(argv[1]);

    int running = 1;
    uint64_t steps = 0;

    while (running) {
        uint32_t inst = mem_read32(pc);

        uint32_t opcode = inst & 0x7f;
        uint32_t rd     = (inst >> 7)  & 0x1f;
        uint32_t funct3 = (inst >> 12) & 0x7;
        uint32_t rs1    = (inst >> 15) & 0x1f;
        uint32_t rs2    = (inst >> 20) & 0x1f;
        uint32_t funct7 = (inst >> 25) & 0x7f;
        int32_t  imm_i  = (int32_t)inst >> 20;  

        uint32_t next_pc = pc + 4;   

        switch (opcode) {
        case 0x13:  
            switch (funct3) {
            case 0x0: regs[rd] = regs[rs1] + imm_i; break;  
            default: illegal(inst);
            }
            break;

        case 0x33:  
            if (funct3 == 0x0 && funct7 == 0x00) {
                regs[rd] = regs[rs1] + regs[rs2];
            }
            else if (funct3 == 0x0 && funct7 == 0x20) {
                regs[rd] = regs[rs1] - regs[rs2];
            }
            else {
                illegal(inst);
            }
            break;

        case 0x73:  
            running = 0;
            break;

        default:
            illegal(inst);
        }

        regs[0] = 0;      
        pc = next_pc;
        steps++;
    }

    printf("Halted after %llu instructions\n", (unsigned long long)steps);
    for (int i = 0; i < 32; i++) {
        printf("x%-2d = 0x%08x (%d)%s", i, regs[i], (int32_t)regs[i],
               (i % 4 == 3) ? "\n" : "   ");
    }
    return 0;
}

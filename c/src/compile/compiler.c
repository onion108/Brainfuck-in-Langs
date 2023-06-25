#include "compiler.h"
#include <_stdio.h>
#include <stdio.h>

int compile(const char *path, Program *p) {
    FILE* fp = fopen(path, "r"); 
    if (!fp) {
        fprintf(stderr, "Cannot open file: %s", path); 
        return 1;
    }

    int c;
    
    // Generate code with uncompleted instructions.
    int is_in_fold = 0;
    int fold_count = 0;

    int counter = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (!is_in_fold) {
            switch (c) {
                case '>':
                    add_instruction(p, BFVM_OP_PR);
                    break;
                case '<':
                    add_instruction(p, BFVM_OP_PL);
                    break;
                case '+':
                    is_in_fold = 1;
                    fold_count = 1;
                    break;
                case '-':
                    is_in_fold = 1;
                    fold_count = -1;
                    break;
                case '.':
                    add_instruction(p, BFVM_OP_OUT);
                    break;
                case ',':
                    add_instruction(p, BFVM_OP_IN);
                    break;
                case '[':
                    add_instruction_uncompleted(p, counter);
                    counter++;
                    break;
                case ']':
                    add_instruction_uncompleted(p, counter-1);
                    counter--;
                    break;
            }
        } else {
            switch (c) {
                case '+':
                    fold_count += 1;
                    break;
                case '-':
                    fold_count -= 1;
                    break;
                default:
                    is_in_fold = 0;
                    // This code is super ugly, but I can't think of a way to 
                    // write it more elegantly.
                    if (fold_count == 1) {
                        add_instruction(p, BFVM_OP_INC);
                    } else if (fold_count == -1) {
                        add_instruction(p, BFVM_OP_DEC);
                    } else if (fold_count > 1) {
                        add_instruction_arg(p, BFVM_OP_ADD, fold_count);
                    } else if (fold_count < -1) {
                        add_instruction_arg(p, BFVM_OP_SUB, -fold_count);
                    } else {
                        // No operation here.
                    }
                    fseek(fp, -1, SEEK_CUR);
                    break;
            }
        }
        if (program_err(p)) {
            fclose(fp);
            return 1;
        }
    }
    if (is_in_fold) {
        is_in_fold = 0;
        // This code is super ugly, but I can't think of a way to 
        // write it more elegantly.
        if (fold_count == 1) {
            add_instruction(p, BFVM_OP_INC);
        } else if (fold_count == -1) {
            add_instruction(p, BFVM_OP_DEC);
        } else if (fold_count > 1) {
            add_instruction_arg(p, BFVM_OP_ADD, fold_count);
        } else if (fold_count < -1) {
            add_instruction_arg(p, BFVM_OP_SUB, -fold_count);
        } else {
            // No operation here.
        }
        if (program_err(p)) {
            return 1;
        }
    }
    fclose(fp);

    // NOTE: Futhur optimizations could be added here since the BFVM_OP_UNCOMPLETED instructions haven't been replaced yet.

    // Complete the uncompleted instructions.
    int virt_pc = 0;
    while (virt_pc < p->len) {
        Instruction *inst = p->instructions[virt_pc];
        if (inst->op_type == BFVM_OP_UNCOMPLETED) {
            int backup_pc = virt_pc;
            int expected_lab = inst->lab;
            int dest_pc = -1;
            virt_pc++;

            // Find the match instruction.
            while (virt_pc < p->len) {
                Instruction *ninst = p->instructions[virt_pc];
                if (ninst->op_type == BFVM_OP_UNCOMPLETED && ninst->lab == expected_lab) {
                    dest_pc = virt_pc;
                    virt_pc = backup_pc;
                    break;
                }
                virt_pc++;
            }
            if (dest_pc == -1) {
                return -1;
            }

            // Replace with the final instruction.
            p->instructions[virt_pc]->op_type = BFVM_OP_JZ;
            p->instructions[virt_pc]->arg = dest_pc;
            p->instructions[virt_pc]->lab = -1;
            p->instructions[dest_pc]->op_type = BFVM_OP_JNZ;
            p->instructions[dest_pc]->arg = virt_pc;
            p->instructions[dest_pc]->lab = -1;
        }
        virt_pc++;
    }
    return 0;
}


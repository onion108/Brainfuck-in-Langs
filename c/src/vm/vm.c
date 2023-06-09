#include "vm.h"
#include <malloc/_malloc.h>
#include <stdio.h>
#include <stdlib.h>

void initialize_program(Program *program) {
    program->instructions = NULL;
    program->len = 0;
}

void add_instruction(Program *program,  OperationType op_type) {
    add_instruction_arg(program, op_type, -1);
}

void add_instruction_arg(Program *program, OperationType op_type, int arg) {
    add_instruction_arg_lab(program, op_type, arg, -1);
}

void add_instruction_uncompleted(Program *program, int lab) {
    add_instruction_arg_lab(program, BFVM_OP_UNCOMPLETED, -1, lab);
}

void add_instruction_arg_lab(Program *program, OperationType op_type, int arg, int lab) {
    // Check if the program is empty or not.
    if (program->instructions) {
        program->instructions = realloc(program->instructions, (program->len + 1) * sizeof(Instruction*));
        if (program->instructions == NULL) {
            program->len = -1;
            return;
        }

        // Create Instruction instance
        Instruction *instruction = malloc(sizeof(Instruction));
        if (!instruction) {
            destruct_program(program);
            program->len = -1;
            return;
        }
        instruction->op_type = op_type;
        instruction->arg = arg;
        instruction->lab = lab;
        program->instructions[program->len] = instruction;
        program->len++;
    } else {
        program->instructions = malloc(sizeof(Instruction*));
        program->len = 1;
        if (!program->instructions) {
            program->len = -1;
            return;
        }

        // Create Instruction instance
        Instruction *instruction = malloc(sizeof(Instruction));
        if (!instruction) {
            // Free the instructions to prevent memory leak.
            free(program->instructions);
            program->len = -1;
            return;
        }
        instruction->op_type = op_type;
        instruction->arg = arg;
        instruction->lab = lab;
        program->instructions[0] = instruction;
    }
}

void destruct_program(Program *program) {
    free(program->instructions);
    program->instructions = NULL;
    program->len = 0;
}

int program_err(Program *program) {
    return program->len == -1;
}

void print_program(Program *program, FILE *fp) {
    fprintf(fp, "; PROGRAM with length %d\n", program->len);
    int idx = 0;
    while (idx < program->len) {
        Instruction *inst = program->instructions[idx];
        switch (inst->op_type) {
            case BFVM_OP_ADD:
                fprintf(fp, "add %d", inst->arg);
                break;
            case BFVM_OP_SUB:
                fprintf(fp, "sub %d", inst->arg);
                break;
            case BFVM_OP_INC:
                fprintf(fp, "inc");
                break;
            case BFVM_OP_DEC:
                fprintf(fp, "dec");
                break;
            case BFVM_OP_JNZ:
                fprintf(fp, "jnz 0x%08X", inst->arg);
                break;
            case BFVM_OP_JZ:
                fprintf(fp, "jz 0x%08X", inst->arg);
                break;
            case BFVM_OP_PL:
                fprintf(fp, "ptr_l");
                break;
            case BFVM_OP_PR:
                fprintf(fp, "ptr_r");
                break;
            case BFVM_OP_IN:
                fprintf(fp, "in");
                break;
            case BFVM_OP_OUT:
                fprintf(fp, "out");
                break;
            case BFVM_OP_UNCOMPLETED:
                fprintf(fp, "?[%d]", inst->lab);
        }
        fprintf(fp, "         ; adrr=%08X\n", idx);
        idx += 1;
    }
}

VM* create_vm(Program program) {
    VM *vm = malloc(sizeof(VM));
    if (vm == NULL) {
        return NULL;
    }
    
    vm->program = program;
    vm->pc = 0;
    vm->ptr = 0;
    vm->mem_positive = malloc(256*sizeof(int));
    if (!vm->mem_positive) {
        free(vm);
        return NULL;
    }
    for (int i = 0; i < 256; i++) {
        vm->mem_positive[i] = 0;
    }
    vm->mem_negative = malloc(256*sizeof(int));
    if (!vm->mem_negative) {
        free(vm);
        return NULL;
    }
    for (int i = 0; i < 256; i++) {
        vm->mem_negative[i] = 0;
    }
    vm->len_mem_negative = 256;
    vm->len_mem_positive = 256;
    return vm;
}

int _vm_prv_rmove_ptr(VM* vm, int step) {
    int dest = vm->ptr + step;
    if (dest > 0) {
        if (dest < vm->len_mem_positive) {
            // Destination fits into the current allocated memory space.
            vm->ptr = dest;
            return 1;
        } else {
            // Destination doesn't fit into the current allocated memory space.
            vm->len_mem_positive = dest + 1;
            vm->mem_positive = realloc(vm->mem_positive, (vm->len_mem_positive) * sizeof(int));
            if (vm->mem_positive == NULL) {
                return 0;
            }
            vm->ptr = dest;
            vm->mem_positive[dest] = 0;
            return 1;
        }
    } else if (dest < 0) {
        int translated_idx = (-dest)-1;
        if (translated_idx < vm->len_mem_negative) {
            // Destination fits into the current allocated memory space for negative indexes.
            vm->ptr = dest;
            return 1;
        } else {
            vm->len_mem_negative = translated_idx + 1;
            vm->mem_negative = realloc(vm->mem_positive, (vm->len_mem_negative) * sizeof(int));
            if (vm->mem_negative == NULL) {
                return 0;
            }
            vm->ptr = dest;
            vm->mem_negative[translated_idx] = 0;
            return 1;
        }
    } else {
        vm->ptr = dest;
        return 1;
    }
    return 1;
}

int *_vm_prv_get_ptr(VM *vm) {
    if (vm->ptr >= 0) {
        return &vm->mem_positive[vm->ptr];
    } else {
        return &vm->mem_negative[(-vm->ptr)-1];
    }
}

void run_vm(VM *vm) {
    vm->pc = 0;
    vm->ptr = 0;
    while (vm->pc < vm->program.len) {
        Instruction* instruction = vm->program.instructions[vm->pc];
        switch (instruction->op_type) {
            case BFVM_OP_UNCOMPLETED:
                fprintf(stderr, "FATAL: Uncompleted instruction at %08X\n", vm->pc);
                return;
            case BFVM_OP_INC:
                *_vm_prv_get_ptr(vm) += 1;
                vm->pc++;
                break;
            case BFVM_OP_DEC:
                *_vm_prv_get_ptr(vm) -= 1;
                vm->pc++;
                break;
            case BFVM_OP_ADD:
                *_vm_prv_get_ptr(vm) += instruction->arg;
                vm->pc++;
                break;
            case BFVM_OP_SUB:
                *_vm_prv_get_ptr(vm) -= instruction->arg;
                vm->pc++;
                break;
            case BFVM_OP_PL:
                if (!_vm_prv_rmove_ptr(vm, -1)) {
                    fprintf(stderr, "FATAL: Cannot allocate memory for the vm to run. \n"); 
                    return;
                }
                vm->pc++;
                break;
            case BFVM_OP_PR:
                if (!_vm_prv_rmove_ptr(vm, 1)) {
                    fprintf(stderr, "FATAL: Cannot allocate memory for the vm to run. \n");
                }
                vm->pc++;
                break;
            case BFVM_OP_JNZ:
                if (*_vm_prv_get_ptr(vm)) {
                    vm->pc = instruction->arg;
                } else {
                    vm->pc++;
                }
                break;
            case BFVM_OP_JZ:
                if (!*_vm_prv_get_ptr(vm)) {
                    vm->pc = instruction->arg;
                } else {
                    vm->pc++;
                }
                break;
            case BFVM_OP_IN:
                *_vm_prv_get_ptr(vm) = getchar();
                vm->pc++;
                break;
            case BFVM_OP_OUT:
                putchar(*_vm_prv_get_ptr(vm));
                vm->pc++;
                break;
        }
    }
}

void destroy_vm(VM *vm) {
    if (vm->mem_negative) {
        free(vm->mem_negative);
    }
    if (vm->mem_positive) {
        free(vm->mem_positive);
    }
    free(vm);
}


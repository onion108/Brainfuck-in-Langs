#ifndef _VM_ONION27_BF_CBFI_H_
#define _VM_ONION27_BF_CBFI_H_

#include <_stdio.h>
typedef enum _OperationType {
    BFVM_OP_INC,
    BFVM_OP_DEC,
    BFVM_OP_ADD, // Reserved instruction for possible optimization
    BFVM_OP_SUB, // Reserved instruction for possible optimization
    BFVM_OP_JZ,
    BFVM_OP_JNZ,
    BFVM_OP_OUT,
    BFVM_OP_IN,
    BFVM_OP_PR,
    BFVM_OP_PL,
    BFVM_OP_UNCOMPLETED,
} OperationType;

typedef struct _Instruction {
    OperationType op_type;
    int arg;
    int lab;
} Instruction;

typedef struct _Program {
    Instruction **instructions;
    int len;
} Program;

void initialize_program(Program *program);
void add_instruction(Program *program, OperationType op_type);
void add_instruction_arg(Program *program, OperationType op_type, int arg);
void add_instruction_arg_lab(Program *program, OperationType op_type, int arg, int lab);
void add_instruction_uncompleted(Program *program, int lab);
void destruct_program(Program *program);
int program_err(Program *program);
void print_program(Program *program, FILE *fp);

typedef struct _VM {
    Program program;
    int pc;
    int ptr;
    int *mem_positive;
    int *mem_negative;
    int len_mem_positive;
    int len_mem_negative;
} VM;

VM* create_vm(Program program);
void run_vm(VM *vm);
void destroy_vm(VM *vm);

#endif


#include "compile/compiler.h"
#include "vm/vm.h"
#include <_stdio.h>
#include <stdio.h>
#include <stdbool.h>

void show_help_line(const char* __fmt, const char* program_name) {
    bool escape = false;

    while (*__fmt) {
        if (!escape) {
            if (*__fmt == '&') {
                escape = true;
            } else {
                putchar(*__fmt);
            }
        } else {
            if (*__fmt == 'N') {
                printf("%s", program_name);
            }
            if (*__fmt == '&') {
                putchar('&');
            }
            escape = false;
        }
        __fmt++;
    }

    putchar('\n');
}

void show_help(const char* program_name) {
    const char *help[] = {
        "Usage: ",
        "&N [source_file]",
        "&N [source_file] [output_file]",
        "Description: ",
        "If one argument is given, run the [source_file] as a Brainfuck source file. It will interprete the content in that file as a Brainfuck source. ",
        "If two arguents are given, it will compile the [source_file] and output the result to the [output_file]. ",
        NULL,
    };
    const char **iter = help;
    while (*iter) {
        show_help_line(*iter, program_name);
        iter++;
    }
}

void test() {
    Program p;
    initialize_program(&p);
    add_instruction_arg(&p, BFVM_OP_ADD, 'H');
    add_instruction(&p, BFVM_OP_OUT);
    add_instruction(&p, BFVM_OP_PL);
    add_instruction_arg(&p, BFVM_OP_ADD, '\n');
    add_instruction(&p, BFVM_OP_OUT);
    print_program(&p, stdout);
    
    VM *vm = create_vm(p);
    run_vm(vm);
    destroy_vm(vm);

    destruct_program(&p);
}

int run(int argc, char** argv) {
    if (argc == 2) {
        Program p;
        initialize_program(&p);
        int err_code = 0;
        if ((err_code = compile(argv[1], &p))) {
            fprintf(stderr, "Internal compilation error: %d", err_code);
            if (!program_err(&p)) {
                destruct_program(&p);
            }
            return -1;
        }

        VM *vm = create_vm(p);
        run_vm(vm);
        destroy_vm(vm);

        destruct_program(&p);
    } else if (argc == 3) {
        Program p;
        initialize_program(&p);
        int err_code = 0;
        if ((err_code = compile(argv[1], &p))) {
            fprintf(stderr, "Internal compilation error: %d\n", err_code);
            if (!program_err(&p)) {
                destruct_program(&p);
            }
            return -1;
        }

        FILE *fp = fopen(argv[2], "w");
        if (fp == NULL) {
            fprintf(stderr, "Cannot open %s\n", argv[2]);
            return -2;
        }
        print_program(&p, fp);
        fclose(fp);

        destruct_program(&p);
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        show_help(argv[0]);
    } else {
        return run(argc, argv);
    }
}


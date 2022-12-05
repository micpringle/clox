//
// Created by Mic Pringle on 03/12/2022.
//

#include "vm.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include <stdio.h>

lox_virtual_machine v_mach;

static void reset_stack() {
    v_mach.stack_next = v_mach.stack;
}

void build_virtual_machine() {
    reset_stack();
}

void purge_virtual_machine() {
}

void push_stack(lox_value value) {
    *v_mach.stack_next = value;
    v_mach.stack_next++;
}

lox_value pop_stack() {
    v_mach.stack_next--;
    return *v_mach.stack_next;
}

static lox_interpret_result run() {
#define READ_BYTE() (*v_mach.instruction_pointer++)
#define READ_CONSTANT() (v_mach.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)           \
    do {                        \
        double b = pop_stack(); \
        double a = pop_stack(); \
        push_stack(a op b);     \
    } while (false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf(" ");
        for (lox_value *value = v_mach.stack; value < v_mach.stack_next; value++) {
            printf("[ ");
            print_value(*value);
            printf(" ]");
        }
        printf("\n");
        disassemble_instruction(v_mach.chunk, (int) (v_mach.instruction_pointer - v_mach.chunk->code));
#endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                lox_value constant = READ_CONSTANT();
                push_stack(constant);
                break;
            }
            case OP_ADD: {
                BINARY_OP(+);
                break;
            }
            case OP_SUBTRACT: {
                BINARY_OP(-);
                break;
            }
            case OP_MULTIPLY: {
                BINARY_OP(*);
                break;
            }
            case OP_DIVIDE: {
                BINARY_OP(/);
                break;
            }
            case OP_NEGATE: {
                push_stack(-pop_stack());
                break;
            }
            case OP_RETURN: {
                print_value(pop_stack());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef BINARY_OP
#undef READ_CONSTANT
#undef READ_BYTE
}

lox_interpret_result interpret_source(const char *source) {
    lox_chunk chunk;
    build_chunk(&chunk);

    if (!compile_source(source, &chunk)) {
        purge_chunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    v_mach.chunk = &chunk;
    v_mach.instruction_pointer = v_mach.chunk->code;

    lox_interpret_result result = run();

    purge_chunk(&chunk);
    return result;
}

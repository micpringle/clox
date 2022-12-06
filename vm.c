//
// Created by Mic Pringle on 03/12/2022.
//

#include "vm.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include <stdarg.h>
#include <stdio.h>

lox_virtual_machine v_mach;

static void reset_stack() {
    v_mach.stack_next = v_mach.stack;
}

static void runtime_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = v_mach.instruction_pointer - v_mach.chunk->code - 1;
    int line_number = v_mach.chunk->line_numbers[instruction];
    fprintf(stderr, "[line %d] in source\n", line_number);
    reset_stack();
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

static lox_value peek_stack(int distance) {
    return v_mach.stack_next[-1 - distance];
}

static bool is_falsey(lox_value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static lox_interpret_result run() {
#define READ_BYTE() (*v_mach.instruction_pointer++)
#define READ_CONSTANT() (v_mach.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(value_type, op)                                     \
    do {                                                              \
        if (!IS_NUMBER(peek_stack(0)) || !IS_NUMBER(peek_stack(1))) { \
            runtime_error("Operands must be numbers.");               \
            return INTERPRET_RUNTIME_ERROR;                           \
        }                                                             \
        double b = AS_NUMBER(pop_stack());                            \
        double a = AS_NUMBER(pop_stack());                            \
        push_stack(value_type(a op b));                               \
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
            case OP_NIL:
                push_stack(NIL_VAL);
                break;
            case OP_TRUE:
                push_stack(BOOL_VAL(true));
                break;
            case OP_FALSE:
                push_stack(BOOL_VAL(false));
                break;
            case OP_EQUAL: {
                lox_value b = pop_stack();
                lox_value a = pop_stack();
                push_stack(BOOL_VAL(values_equal(a, b)));
                break;
            }
            case OP_GREATER:
                BINARY_OP(BOOL_VAL, >);
                break;
            case OP_LESS:
                BINARY_OP(BOOL_VAL, <);
                break;
            case OP_ADD:
                BINARY_OP(NUMBER_VAL, +);
                break;
            case OP_SUBTRACT:
                BINARY_OP(NUMBER_VAL, -);
                break;
            case OP_MULTIPLY:
                BINARY_OP(NUMBER_VAL, *);
                break;
            case OP_DIVIDE:
                BINARY_OP(NUMBER_VAL, /);
                break;
            case OP_NOT:
                push_stack(BOOL_VAL(is_falsey(pop_stack())));
                break;
            case OP_NEGATE:
                if (!IS_NUMBER(peek_stack(0))) {
                    runtime_error("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push_stack(NUMBER_VAL(-AS_NUMBER(pop_stack())));
                break;
            case OP_RETURN:
                print_value(pop_stack());
                printf("\n");
                return INTERPRET_OK;
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

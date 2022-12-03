//
// Created by Mic Pringle on 03/12/2022.
//

#include <stdio.h>
#include "debug.h"
#include "value.h"

void disassemble_chunk(lox_chunk *chunk, const char* name) {
    printf("-- %s --\n", name);
    for (int offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
}

static int constant_instruction(const char *name, lox_chunk *chunk, int offset) {
    uint8_t index = chunk->code[offset + 1];
    printf("%-16s %4d '", name, index);
    print_value(chunk->constants.values[index]);
    printf("\n");
    return offset + 2;
}

static int simple_instruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int  disassemble_instruction(lox_chunk *chunk, int offset) {
    printf("%04d ", offset);
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constant_instruction("OP_CONSTANT", chunk, offset);
        case OP_RETURN:
            return simple_instruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d.\n", instruction);
            return offset + 1;
    }
}

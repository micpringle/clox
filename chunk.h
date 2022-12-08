//
// Created by Mic Pringle on 03/12/2022.
//

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,
    OP_PRINT,
    OP_RETURN
} lox_op_code;

typedef struct {
    int capacity;
    int count;
    uint8_t *code;
    int *line_numbers;
    lox_value_array constants;
} lox_chunk;

void build_chunk(lox_chunk *chunk);
void purge_chunk(lox_chunk *chunk);
void write_chunk(lox_chunk *chunk, uint8_t byte, int line_number);

int add_constant(lox_chunk *chunk, lox_value constant);

#endif

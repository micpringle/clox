//
// Created by Mic Pringle on 03/12/2022.
//

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN
} lox_op_code;

typedef struct {
    int capacity;
    int count;
    uint8_t *code;
    lox_value_array constants;
} lox_chunk;

void build_chunk(lox_chunk *chunk);
void purge_chunk(lox_chunk *chunk);
void write_chunk(lox_chunk *chunk, uint8_t byte);

int add_constant(lox_chunk *chunk, lox_value constant);

#endif
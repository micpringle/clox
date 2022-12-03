//
// Created by Mic Pringle on 03/12/2022.
//

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"

typedef enum {
    OP_RETURN
} lox_op_code;

typedef struct {
    int count;
    int capacity;
    uint8_t *code;
} lox_chunk;

void build_chunk(lox_chunk *chunk);
void purge_chunk(lox_chunk *chunk);
void write_chunk(lox_chunk *chunk, uint8_t byte);

#endif

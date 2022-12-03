//
// Created by Mic Pringle on 03/12/2022.
//

#include <stdlib.h>
#include "memory.h"
#include "chunk.h"

void build_chunk(lox_chunk *chunk) {
    chunk->capacity = 0;
    chunk->count = 0;
    chunk->code = NULL;
    build_value_array(&chunk->constants);
}

void purge_chunk(lox_chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    purge_value_array(&chunk->constants);
    build_chunk(chunk);
}

void write_chunk(lox_chunk *chunk, uint8_t byte) {
    if (chunk->capacity < chunk->count + 1) {
        int capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, capacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;
}

int add_constant(lox_chunk *chunk, lox_value constant) {
    write_value_array(&chunk->constants, constant);
    return chunk->constants.count - 1;
}

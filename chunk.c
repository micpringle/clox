//
// Created by Mic Pringle on 03/12/2022.
//

#include "chunk.h"
#include "memory.h"

void build_chunk(lox_chunk *chunk) {
    chunk->capacity = 0;
    chunk->count = 0;
    chunk->code = NULL;
    chunk->line_numbers = NULL;
    build_value_array(&chunk->constants);
}

void purge_chunk(lox_chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->line_numbers, chunk->capacity);
    purge_value_array(&chunk->constants);
    build_chunk(chunk);
}

void write_chunk(lox_chunk *chunk, uint8_t byte, int line_number) {
    if (chunk->capacity < chunk->count + 1) {
        int capacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, capacity, chunk->capacity);
        chunk->line_numbers = GROW_ARRAY(int, chunk->line_numbers, capacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->line_numbers[chunk->count] = line_number;
    chunk->count++;
}

int add_constant(lox_chunk *chunk, lox_value constant) {
    write_value_array(&chunk->constants, constant);
    return chunk->constants.count - 1;
}

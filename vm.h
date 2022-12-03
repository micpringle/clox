//
// Created by Mic Pringle on 03/12/2022.
//

#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    lox_chunk *chunk;
    uint8_t *instruction_pointer;
    lox_value stack[STACK_MAX];
    lox_value *stack_next;
} lox_virtual_machine;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} lox_interpret_result;

void build_virtual_machine();
void purge_virtual_machine();

lox_interpret_result interpret_chunk(lox_chunk *chunk);

void push_stack(lox_value value);
lox_value pop_stack();

#endif

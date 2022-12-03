//
// Created by Mic Pringle on 03/12/2022.
//

#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"

void disassemble_chunk(lox_chunk *chunk, const char* name);
int disassemble_instruction(lox_chunk *chunk, int offset);

#endif

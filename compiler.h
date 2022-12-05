//
// Created by Mic Pringle on 05/12/2022.
//

#ifndef CLOX_COMPILER_H
#define CLOX_COMPILER_H

#include "vm.h"

bool compile_source(const char *source, lox_chunk *chunk);

#endif

//
// Created by Mic Pringle on 05/12/2022.
//

#ifndef CLOX_COMPILER_H
#define CLOX_COMPILER_H

#include "object.h"
#include "vm.h"

ObjFunction *compile(const char *source);

void markCompilerRoots();

#endif

//
// Created by Mic Pringle on 03/12/2022.
//

#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

typedef double lox_value;

typedef struct {
    int capacity;
    int count;
    lox_value *values;
} lox_value_array;

void build_value_array(lox_value_array *array);
void write_value_array(lox_value_array *array, lox_value value);
void purge_value_array(lox_value_array *array);

void print_value(lox_value value);

#endif

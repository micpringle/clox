//
// Created by Mic Pringle on 03/12/2022.
//

#include "value.h"
#include "memory.h"
#include <stdio.h>

void build_value_array(lox_value_array *array) {
    array->capacity = 0;
    array->count = 0;
    array->values = NULL;
}

void write_value_array(lox_value_array *array, lox_value value) {
    if (array->capacity < array->count + 1) {
        int capacity = array->capacity;
        array->capacity = GROW_CAPACITY(capacity);
        array->values = GROW_ARRAY(lox_value, array->values, capacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void purge_value_array(lox_value_array *array) {
    FREE_ARRAY(lox_value, array->values, array->capacity);
    build_value_array(array);
}

void print_value(lox_value value) {
    printf("%g", value);
}

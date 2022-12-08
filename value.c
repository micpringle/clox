//
// Created by Mic Pringle on 03/12/2022.
//

#include "value.h"
#include "memory.h"
#include "object.h"

#include <stdio.h>
#include <string.h>

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
    switch (value.type) {
        case VAL_BOOL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case VAL_NIL:
            printf("nil");
            break;
        case VAL_NUMBER:
            printf("%g", AS_NUMBER(value));
            break;
        case VAL_OBJECT:
            print_object(value);
            break;
    }
}

bool values_equal(lox_value lhs, lox_value rhs) {
    if (lhs.type != rhs.type) return false;
    switch (lhs.type) {
        case VAL_BOOL:
            return AS_BOOL(lhs) == AS_BOOL(rhs);
        case VAL_NIL:
            return true;
        case VAL_NUMBER:
            return AS_NUMBER(lhs) == AS_NUMBER(rhs);
        case VAL_OBJECT: {
            lox_string *lhs_string = AS_STRING(lhs);
            lox_string *rhs_string = AS_STRING(rhs);
            return lhs_string->length == rhs_string->length &&
                memcmp(lhs_string->characters, rhs_string->characters, lhs_string->length) == 0;
        }
        default:
            return false;
    }
}

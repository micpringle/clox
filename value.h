//
// Created by Mic Pringle on 03/12/2022.
//

#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

typedef struct lox_object lox_object;
typedef struct lox_string lox_string;

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJECT
} lox_value_type;

typedef struct {
    lox_value_type type;
    union {
        bool boolean;
        double number;
        lox_object *object;
    } as;
} lox_value;

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_OBJECT(value) ((value).type == VAL_OBJECT)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJECT(value) ((value).as.object)

#define BOOL_VAL(value) ((lox_value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((lox_value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((lox_value){VAL_NUMBER, {.number = value}})
#define OBJECT_VAL(value) ((lox_value){VAL_OBJECT, {.object = (lox_object *)value}})

typedef struct {
    int capacity;
    int count;
    lox_value *values;
} lox_value_array;

bool values_equal(lox_value lhs, lox_value rhs);

void build_value_array(lox_value_array *array);
void write_value_array(lox_value_array *array, lox_value value);
void purge_value_array(lox_value_array *array);

void print_value(lox_value value);

#endif

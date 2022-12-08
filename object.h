//
// Created by Mic Pringle on 07/12/2022.
//

#ifndef CLOX_OBJECT_H
#define CLOX_OBJECT_H

#include "common.h"
#include "value.h"

#define OBJECT_TYPE(value) (AS_OBJECT(value)->type)

#define IS_STRING(value) is_object_type(value, OBJ_STRING)

#define AS_STRING(value) ((lox_string *)AS_OBJECT(value))
#define AS_CSTRING(value) (((lox_string *)AS_OBJECT(value))->characters)

typedef enum {
    OBJ_STRING
} lox_object_type;

struct lox_object {
    lox_object_type type;
    struct lox_object *next_object;
};

struct lox_string {
    lox_object object;
    int length;
    char *characters;
    uint32_t hash;
};

lox_string *take_string(char *characters, int length);
lox_string *copy_string(const char *characters, int length);
void print_object(lox_value value);

static inline bool is_object_type(lox_value value, lox_object_type type) {
    return IS_OBJECT(value) && AS_OBJECT(value)->type == type;
}

#endif

//
// Created by Mic Pringle on 07/12/2022.
//

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#include <stdio.h>
#include <string.h>

#define ALLOCATE_OBJ(type, object_type) (type *) allocate_object(sizeof(type), object_type)

static lox_object *allocate_object(size_t size, lox_object_type type) {
    lox_object *object = (lox_object *) reallocate(NULL, 0, size);
    object->type = type;
    return object;
}

static lox_string *allocate_string(char *characters, int length) {
    lox_string *string = ALLOCATE_OBJ(lox_string, OBJ_STRING);
    string->length = length;
    string->characters = characters;
    return string;
}

lox_string *copy_string(const char *characters, int length) {
    char *buffer = ALLOCATE(char, length + 1);
    memcpy(buffer, characters, length);
    buffer[length] = '\0';
    return allocate_string(buffer, length);
}
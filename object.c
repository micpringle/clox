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
    object->next_object = v_mach.object_list_head;
    v_mach.object_list_head = object;
    return object;
}

static lox_string *allocate_string(char *characters, int length) {
    lox_string *string = ALLOCATE_OBJ(lox_string, OBJ_STRING);
    string->length = length;
    string->characters = characters;
    return string;
}

lox_string *take_string(char *characters, int length) {
    return allocate_string(characters, length);
}

lox_string *copy_string(const char *characters, int length) {
    char *buffer = ALLOCATE(char, length + 1);
    memcpy(buffer, characters, length);
    buffer[length] = '\0';
    return allocate_string(buffer, length);
}

void print_object(lox_value value) {
    switch (OBJECT_TYPE(value)) {
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
    }
}

//
// Created by Mic Pringle on 03/12/2022.
//

#include "memory.h"
#include "vm.h"

#include <stdlib.h>

void *reallocate(void *pointer, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, new_size);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

static void purge_object(lox_object *object) {
    switch (object->type) {
        case OBJ_STRING: {
            lox_string *string = (lox_string *)object;
            FREE_ARRAY(char, string->characters, string->length + 1);
            FREE(lox_string, object);
            break;
        }
    }
}

void purge_objects() {
    lox_object *object = v_mach.object_list_head;
    while (object != NULL) {
        lox_object *next_object = object->next_object;
        purge_object(object);
        object = next_object;
    }
}

//
// Created by Mic Pringle on 08/12/2022.
//

#include "memory.h"
#include "object.h"
#include "value.h"
#include "table.h"

#include <stdlib.h>
#include <string.h>

#define TABLE_MAX_LOAD 0.75

void build_table(lox_table *table) {
    table->capacity = 0;
    table->count = 0;
    table->rows = NULL;
}

void purge_table(lox_table *table) {
    FREE_ARRAY(lox_table_row, table->rows, table->capacity);
    build_table(table);
}

static lox_table_row *find_table_row(lox_table_row *rows, int capacity, lox_string *key) {
    uint32_t index = key->hash % capacity;
    lox_table_row *tombstone = NULL;

    for (;;) {
        lox_table_row *row = &rows[index];

        if (row->key == NULL) {
            if (IS_NIL(row->value)) {
                return tombstone != NULL ? tombstone : row;
            } else {
                if (tombstone == NULL) tombstone = row;
            }
        } else if (row->key == key) {
            return row;
        }

        index = (index + 1) % capacity;
    }
}

static void adjust_table_capacity(lox_table *table, int capacity) {
    lox_table_row *rows = ALLOCATE(lox_table_row, capacity);
    for (int i = 0; i < capacity; i++) {
        rows[i].key = NULL;
        rows[i].value = NIL_VAL;
    }

    table->count = 0;
    for (int i = 0; i < table->capacity; i++) {
        lox_table_row *old_row = &table->rows[i];
        if (old_row->key == NULL) continue;

        lox_table_row *new_row = find_table_row(rows, capacity, old_row->key);
        new_row->key = old_row->key;
        new_row->value = old_row->value;
        table->count++;
    }

    FREE_ARRAY(lox_table_row, table->rows, table->capacity);

    table->rows = rows;
    table->capacity = capacity;
}

bool get_table_row(lox_table *table, lox_string *key, lox_value *value) {
    if (table->count == 0) return false;

    lox_table_row *row = find_table_row(table->rows, table->capacity, key);
    if (row->key == NULL) return false;

    *value = row->value;
    return true;
}

bool set_table_row(lox_table *table, lox_string *key, lox_value value) {
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjust_table_capacity(table, capacity);
    }

    lox_table_row *row = find_table_row(table->rows, table->capacity, key);
    bool is_new_key = row->key == NULL;
    if (is_new_key && IS_NIL(row->value)) table->count++;

    row->key = key;
    row->value = value;
    return is_new_key;
}

bool rip_table_row(lox_table *table, lox_string *key) {
    if (table->count == 0) return false;

    lox_table_row *row = find_table_row(table->rows, table->capacity, key);
    if (row->key == NULL) return false;

    row->key = NULL;
    row->value = BOOL_VAL(true);
    return true;
}

void copy_table(lox_table *source, lox_table *destination) {
    for (int i = 0; i < source->capacity; i++) {
        lox_table_row *row = &source->rows[i];
        if (row->key != NULL) {
            set_table_row(destination, row->key, row->value);
        }
    }
}

//
// Created by Mic Pringle on 08/12/2022.
//

#ifndef CLOX_TABLE_H
#define CLOX_TABLE_H

#include "common.h"
#include "value.h"

typedef struct {
    lox_string *key;
    lox_value value;
} lox_table_row;

typedef struct {
    int capacity;
    int count;
    lox_table_row *rows;
} lox_table;

void build_table(lox_table *table);
void purge_table(lox_table *table);
void copy_table(lox_table *source, lox_table *destination);

bool get_table_row(lox_table *table, lox_string *key, lox_value *value);
bool set_table_row(lox_table *table, lox_string *key, lox_value value);
bool rip_table_row(lox_table *table, lox_string *key);

#endif

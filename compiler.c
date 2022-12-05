//
// Created by Mic Pringle on 05/12/2022.
//

#include <stdio.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"

void compile_source(const char *source) {
    build_scanner(source);

    int line_number = -1;
    for (;;) {
        lox_token token = next_token();
        if (token.line_number != line_number) {
            printf("%4d ", token.line_number);
            line_number = token.line_number;
        } else {
            printf(" | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}
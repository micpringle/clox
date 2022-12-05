//
// Created by Mic Pringle on 05/12/2022.
//

#ifndef CLOX_SCANNER_H
#define CLOX_SCANNER_H

typedef enum {
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_PERIOD,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,
    TOKEN_BANG,
    TOKEN_NOT_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUALITY,
    TOKEN_GREATER_THAN,
    TOKEN_GREATER_THAN_EQUAL,
    TOKEN_LESS_THAN,
    TOKEN_LESS_THAN_EQUAL,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_AND,
    TOKEN_CLASS,
    TOKEN_ELSE,
    TOKEN_FALSE,
    TOKEN_FOR,
    TOKEN_FUN,
    TOKEN_IF,
    TOKEN_NIL,
    TOKEN_OR,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_SUPER,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VAR,
    TOKEN_WHILE,
    TOKEN_ERROR,
    TOKEN_EOF
} lox_token_type;

typedef struct {
    lox_token_type type;
    const char *start;
    int length;
    int line_number;
} lox_token;

void build_scanner(const char *source);
lox_token next_token();

#endif

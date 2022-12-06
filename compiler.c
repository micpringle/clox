//
// Created by Mic Pringle on 05/12/2022.
//

#include "compiler.h"
#include "common.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

typedef struct {
    lox_token current_token;
    lox_token previous_token;
    bool errored;
    bool is_panicked;
} lox_parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,
    PREC_OR,
    PREC_AND,
    PREC_EQUALITY,
    PREC_COMPARISON,
    PREC_TERM,
    PREC_FACTOR,
    PREC_UNARY,
    PREC_CALL,
    PREC_PRIMARY
} lox_precedence;

typedef void (*lox_parse_function)();

typedef struct {
    lox_parse_function prefix;
    lox_parse_function infix;
    lox_precedence precedence;
} lox_parse_rule;

lox_parser parser;
lox_chunk *compiling_chunk;

static void parse_expression();
static lox_parse_rule *lookup_rule(lox_token_type type);
static void parse_precedence(lox_precedence precedence);

static lox_chunk *current_chunk() {
    return compiling_chunk;
}

static void error_at(lox_token *token, const char *message) {
    if (parser.is_panicked) return;
    parser.is_panicked = true;
    fprintf(stderr, "[line %d] error", token->line_number);
    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        // Do nothing, for now?
    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }
    fprintf(stderr, ": %s\n", message);
    parser.errored = true;
}

static void error_at_current(const char *message) {
    error_at(&parser.current_token, message);
}

static void error(const char *message) {
    error_at(&parser.previous_token, message);
}

static void process_token() {
    parser.previous_token = parser.current_token;
    for (;;) {
        parser.current_token = next_token();
        if (parser.current_token.type != TOKEN_ERROR) break;
        error_at_current(parser.current_token.start);
    }
}

static void consume_token(lox_token_type type, const char *message) {
    if (parser.current_token.type == type) {
        process_token();
        return;
    }
    error_at_current(message);
}

static void emit_byte(uint8_t op_code) {
    write_chunk(current_chunk(), op_code, parser.previous_token.line_number);
}

static void emit_bytes(uint8_t op_code, uint8_t operand) {
    emit_byte(op_code);
    emit_byte(operand);
}

static void emit_return() {
    emit_byte(OP_RETURN);
}

static uint8_t make_constant(lox_value value) {
    int index = add_constant(current_chunk(), value);
    if (index > UINT8_MAX) {
        error("Too many constants in a single chunk.");
        return 0;
    }
    return (u_int8_t) index;
}

static void emit_constant(lox_value value) {
    emit_bytes(OP_CONSTANT, make_constant(value));
}

static void finish_compilation() {
    emit_return();
#ifdef DEBUG_PRINT_CODE
    if (!parser.errored) {
        disassemble_chunk(current_chunk(), "code");
    }
#endif
}

static void parse_binary() {
    lox_token_type operator = parser.previous_token.type;
    lox_parse_rule *rule = lookup_rule(operator);
    parse_precedence((lox_precedence) rule->precedence + 1);
    switch (operator) {
        case TOKEN_PLUS:
            emit_byte(OP_ADD);
            break;
        case TOKEN_MINUS:
            emit_byte(OP_SUBTRACT);
            break;
        case TOKEN_STAR:
            emit_byte(OP_MULTIPLY);
            break;
        case TOKEN_SLASH:
            emit_byte(OP_DIVIDE);
            break;
        default:
            return;
    }
}

static void parse_group() {
    parse_expression();
    consume_token(TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
}

static void parse_number() {
    double value = strtod(parser.previous_token.start, NULL);
    emit_constant(NUMBER_VAL(value));
}

static void parse_unary() {
    lox_token_type operator = parser.previous_token.type;
    parse_precedence(PREC_UNARY);
    switch (operator) {
        case TOKEN_MINUS:
            emit_byte(OP_NEGATE);
            break;
        default:
            return;
    }
}

lox_parse_rule rules[] = {
        [TOKEN_LEFT_PAREN] = {parse_group, NULL, PREC_NONE},
        [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
        [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
        [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
        [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
        [TOKEN_PERIOD] = {NULL, NULL, PREC_NONE},
        [TOKEN_MINUS] = {parse_unary, parse_binary, PREC_TERM},
        [TOKEN_PLUS] = {NULL, parse_binary, PREC_TERM},
        [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
        [TOKEN_SLASH] = {NULL, parse_binary, PREC_FACTOR},
        [TOKEN_STAR] = {NULL, parse_binary, PREC_FACTOR},
        [TOKEN_BANG] = {NULL, NULL, PREC_NONE},
        [TOKEN_NOT_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_EQUALITY] = {NULL, NULL, PREC_NONE},
        [TOKEN_GREATER_THAN] = {NULL, NULL, PREC_NONE},
        [TOKEN_GREATER_THAN_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_LESS_THAN] = {NULL, NULL, PREC_NONE},
        [TOKEN_LESS_THAN_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
        [TOKEN_STRING] = {NULL, NULL, PREC_NONE},
        [TOKEN_NUMBER] = {parse_number, NULL, PREC_NONE},
        [TOKEN_AND] = {NULL, NULL, PREC_NONE},
        [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
        [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
        [TOKEN_FALSE] = {NULL, NULL, PREC_NONE},
        [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
        [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
        [TOKEN_IF] = {NULL, NULL, PREC_NONE},
        [TOKEN_NIL] = {NULL, NULL, PREC_NONE},
        [TOKEN_OR] = {NULL, NULL, PREC_NONE},
        [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
        [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
        [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
        [TOKEN_THIS] = {NULL, NULL, PREC_NONE},
        [TOKEN_TRUE] = {NULL, NULL, PREC_NONE},
        [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
        [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
        [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
        [TOKEN_EOF] = {NULL, NULL, PREC_NONE}};

static void parse_precedence(lox_precedence precedence) {
    process_token();
    lox_parse_function prefix_rule = lookup_rule(parser.previous_token.type)->prefix;
    if (prefix_rule == NULL) {
        error("Expected expression.");
        return;
    }
    prefix_rule();

    while (precedence <= lookup_rule(parser.current_token.type)->precedence) {
        process_token();
        lox_parse_function infix_rule = lookup_rule(parser.previous_token.type)->infix;
        infix_rule();
    }
}

static lox_parse_rule *lookup_rule(lox_token_type type) {
    return &rules[type];
}

static void parse_expression() {
    parse_precedence(PREC_ASSIGNMENT);
}

bool compile_source(const char *source, lox_chunk *chunk) {
    build_scanner(source);
    compiling_chunk = chunk;
    parser.errored = false;
    parser.is_panicked = false;

    process_token();
    parse_expression();
    consume_token(TOKEN_EOF, "Expected end of expression.");
    finish_compilation();

    return !parser.errored;
}

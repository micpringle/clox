//
// Created by Mic Pringle on 05/12/2022.
//

#include "scanner.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    const char *start;
    const char *current;
    int line_number;
} lox_scanner;

lox_scanner scanner;

void build_scanner(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line_number = 1;
}

static bool is_finished() {
    return *scanner.current == '\0';
}

static bool is_number(char value) {
    return value >= '0' && value <= '9';
}

static bool is_letter(char value) {
    return (value >= 'a' && value <= 'z') ||
           (value >= 'A' && value <= 'Z') ||
           value == '_';
}

static char next_character() {
    scanner.current++;
    return scanner.current[-1];
}

static bool next_character_matches(char expected) {
    if (is_finished() || *scanner.current != expected) return false;
    scanner.current++;
    return true;
}

static char peek() {
    return *scanner.current;
}

static char peek_next() {
    if (is_finished()) return '\0';
    return scanner.current[1];
}

static void ignore_whitespace() {
    for (;;) {
        char next = peek();
        switch (next) {
            case ' ':
            case '\r':
            case '\t':
                next_character();
                break;
            case '\n':
                scanner.line_number++;
                next_character();
                break;
            case '/':
                if (peek_next() == '/') {
                    while (peek() != '\n' && !is_finished())
                        next_character();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static lox_token_type check_for_keyword(int start, int length, const char *remaining, lox_token_type type) {
    if (scanner.current - scanner.start == start + length && memcmp(scanner.start + start, remaining, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static lox_token_type determine_identifier_type() {
    switch (scanner.start[0]) {
        case 'a':
            return check_for_keyword(1, 2, "nd", TOKEN_AND);
        case 'c':
            return check_for_keyword(1, 4, "lass", TOKEN_CLASS);
        case 'e':
            return check_for_keyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a':
                        return check_for_keyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o':
                        return check_for_keyword(2, 1, "r", TOKEN_FOR);
                    case 'u':
                        return check_for_keyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i':
            return check_for_keyword(1, 1, "f", TOKEN_IF);
        case 'n':
            return check_for_keyword(1, 2, "il", TOKEN_NIL);
        case 'o':
            return check_for_keyword(1, 1, "r", TOKEN_OR);
        case 'p':
            return check_for_keyword(1, 4, "rint", TOKEN_PRINT);
        case 'r':
            return check_for_keyword(1, 5, "eturn", TOKEN_RETURN);
        case 's':
            return check_for_keyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h':
                        return check_for_keyword(2, 2, "is", TOKEN_THIS);
                    case 'r':
                        return check_for_keyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v':
            return check_for_keyword(1, 2, "ar", TOKEN_VAR);
        case 'w':
            return check_for_keyword(1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static lox_token make_token(lox_token_type type) {
    lox_token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int) (scanner.current - scanner.start);
    token.line_number = scanner.line_number;
    return token;
}

static lox_token make_error_token(const char *message) {
    lox_token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int) strlen(message);
    token.line_number = scanner.line_number;
    return token;
}

static lox_token make_string_token() {
    while (peek() != '"' && !is_finished()) {
        if (peek() == '\n') scanner.line_number++;
        next_character();
    }
    if (is_finished()) return make_error_token("Unterminated string.");
    next_character();
    return make_token(TOKEN_STRING);
}

static lox_token make_number_token() {
    while (is_number(peek())) next_character();
    if (peek() == '.' && is_number(peek_next())) {
        next_character();
        while (is_number(peek())) next_character();
    }
    return make_token(TOKEN_NUMBER);
}

static lox_token make_identifier_token() {
    while (is_letter(peek()) || is_number(peek())) next_character();
    return make_token(determine_identifier_type());
}

lox_token next_token() {
    ignore_whitespace();
    scanner.start = scanner.current;
    if (is_finished()) return make_token(TOKEN_EOF);

    char next = next_character();
    if (is_letter(next)) return make_identifier_token();
    if (is_number(next)) return make_number_token();
    switch (next) {
        case '(':
            return make_token(TOKEN_LEFT_PAREN);
        case ')':
            return make_token(TOKEN_RIGHT_PAREN);
        case '{':
            return make_token(TOKEN_LEFT_BRACE);
        case '}':
            return make_token(TOKEN_RIGHT_BRACE);
        case ';':
            return make_token(TOKEN_SEMICOLON);
        case ',':
            return make_token(TOKEN_COMMA);
        case '.':
            return make_token(TOKEN_PERIOD);
        case '-':
            return make_token(TOKEN_MINUS);
        case '+':
            return make_token(TOKEN_PLUS);
        case '/':
            return make_token(TOKEN_SLASH);
        case '*':
            return make_token(TOKEN_STAR);
        case '!':
            return make_token(next_character_matches('=') ? TOKEN_NOT_EQUAL : TOKEN_BANG);
        case '=':
            return make_token(next_character_matches('=') ? TOKEN_EQUALITY : TOKEN_EQUAL);
        case '<':
            return make_token(next_character_matches('=') ? TOKEN_LESS_THAN_EQUAL : TOKEN_LESS_THAN);
        case '>':
            return make_token(next_character_matches('=') ? TOKEN_GREATER_THAN_EQUAL : TOKEN_GREATER_THAN);
        case '"':
            return make_string_token();
        default:
            break;
    }

    return make_error_token("Unexpected character");
}
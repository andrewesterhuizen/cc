#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

#include "lexer.h"
#include "token.h"
#include "string_builder.h"

const int len_keywords = 3;
char *keywords[len_keywords] = {
        "if",
        "for",
        "while",
};

const int len_data_types = 1;
char *data_types[len_data_types] = {
        "int",
};


int is_keyword(char *identifier) {
    for (int i = 0; i < len_keywords; i++) {
        int cmp = strcmp(identifier, keywords[i]);
        if (cmp == 0) {
            return 1;
        }
    }

    return 0;
}

int is_data_type(char *identifier) {
    for (int i = 0; i < len_data_types; i++) {
        int cmp = strcmp(identifier, data_types[i]);
        if (cmp == 0) {
            return 1;
        }
    }

    return 0;
}

token_t *get_integer_token(char *input, int *index) {
    string_builder_t *sb = string_builder_new();

    while (input[*index] != '\0') {
        char c = input[*index];

        if (isnumber(c)) {
            string_builder_append_char(sb, c);
        } else {
            break;
        }

        (*index)++;
    }

    char *s = string_builder_get_string_copy(sb);
    string_builder_destroy(sb);

    return new_token(TokenTypeIntegerLiteral, s);
}

token_t *get_string_token(char *input, int *index) {
    string_builder_t *sb = string_builder_new();

    while (input[*index] != '\0') {
        char c = input[*index];

        if (isalnum(c)) {
            string_builder_append_char(sb, c);
        } else {
            break;
        }

        (*index)++;
    }

    char *s = string_builder_get_string_copy(sb);
    string_builder_destroy(sb);

    if (is_keyword(s)) {
        return new_token(TokenTypeKeyword, s);
    } else if (is_data_type(s)) {
        return new_token(TokenTypeDataType, s);
    }

    return new_token(TokenTypeIdentifier, s);
}

token_t *getTokens(char *input) {
    token_t *token_head = new_token(TokenTypeStart, "");
    token_t *token_tail = token_head;

    int index = 0;

    while (input[index] != '\0') {
        char c = input[index];

        if (isnumber(c)) {
            token_t *t = get_integer_token(input, &index);
            token_tail->next = t;
            token_tail = t;
            continue;
        }

        if (isalnum(c)) {
            token_t *t = get_string_token(input, &index);
            token_tail->next = t;
            token_tail = t;
            continue;
        }

        switch (c) {
            case ';':
                token_tail = token_add_next(token_tail, TokenTypeSemiColon, ";");
                break;
            case '=':
                token_tail = token_add_next(token_tail, TokenTypeEquals, "=");
                break;
            case '+':
                token_tail = token_add_next(token_tail, TokenTypePlus, "+");
                break;
            case '-':
                token_tail = token_add_next(token_tail, TokenTypeMinus, "-");
                break;
            case '*':
                token_tail = token_add_next(token_tail, TokenTypeAsterisk, "*");
                break;
            case '/':
                token_tail = token_add_next(token_tail, TokenTypeForwardSlash, "/");
                break;
            case '(':
                token_tail = token_add_next(token_tail, TokenTypeLeftParen, "(");
                break;
            case ')':
                token_tail = token_add_next(token_tail, TokenTypeRightParen, ")");
                break;
            case '{':
                token_tail = token_add_next(token_tail, TokenTypeLeftBrace, "{");
                break;
            case '}':
                token_tail = token_add_next(token_tail, TokenTypeRightBrace, "}");
                break;
            case '<':
                token_tail = token_add_next(token_tail, TokenTypeAngleBracketLeft, "<");
                break;
            case '>':
                token_tail = token_add_next(token_tail, TokenTypeAngleBracketRight, ">");
                break;
            case ' ':
            case '\n':
                // skip
                break;
            default:
                fprintf(stderr, "lexing error: unknown char = \"%c\", code = (%d)\n", c, c);
                exit(1);
        }

        index++;
    }

    token_tail = token_add_next(token_tail, TokenTypeEndOfFile, "");

    return token_head;
}
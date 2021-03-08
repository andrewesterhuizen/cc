#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>

#include "lexer.h"
#include "token.h"
#include "string_builder.h"
#include "defines.h"

const int len_keywords = 4;
char *keywords[len_keywords] = {
        "if",
        "for",
        "while",
        "return"
};

const int len_data_types = 3;
char *data_types[len_data_types] = {
        "int",
        "char",
        "void"
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

void skip_whitespace(char *input, int *index) {
    while (input[*index] != '\0' && input[*index] == ' ') {
        (*index)++;
    }

    return;
}

char *get_chars_until(char *input, int *index, char c) {
    string_builder_t *sb = string_builder_new();

    while (input[*index] != '\0' && input[*index] != c) {
        string_builder_append_char(sb, input[*index]);
        (*index)++;
    }

    char *str = string_builder_get_string_copy(sb);
    string_builder_destroy(sb);

    return str;
}

token_t *get_preproccesor_directive(char *input, int *index) {
    string_builder_t *sb = string_builder_new();

    int i = *index;

    // get directive
    while (input[i] != '\0' && input[i] != ' ') {
        string_builder_append_char(sb, input[i]);
        i++;
    }

    char *directive = string_builder_get_string_copy(sb);

    // skip whitespace
    while (input[i] != '\0' && input[i] == ' ') {
        i++;
    }

    string_builder_reset(sb);

    // get name
    while (input[i] != '\0' && input[i] == '<' || input[i] == '>' || input[i] == '.' || isalnum(input[i])) {
        string_builder_append_char(sb, input[i]);
        i++;
    }

    *index = i;

    char *name = string_builder_get_string_copy(sb);
    string_builder_destroy(sb);

    if (strcmp("include", directive) == 0) {
        token_t *t = new_token(TokenTypePreprocessorInclude, name);
        return t;
    }

    if (strcmp("define", directive) == 0) {
        skip_whitespace(input, index);
        char *value = get_chars_until(input, index, '\n');
        token_t *t = new_token(TokenTypePreprocessorDefine, name);
        t->extra_value = value;
        return t;
    }

    EXIT_ERRORF("unexpected preprocessor directive: %s\n", directive);
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
            // preprocessor directive
            case '#':
                index++;
                token_tail->next = get_preproccesor_directive(input, &index);
                token_tail = token_tail->next;
                break;

            case '"': {
                // string
                index++;
                char *value = get_chars_until(input, &index, '"');
                token_tail = token_add_next(token_tail, TokenTypeStringLiteral, value);
                break;
            }

            case ';':
                token_tail = token_add_next(token_tail, TokenTypeSemiColon, ";");
                break;
            case ',':
                token_tail = token_add_next(token_tail, TokenTypeComma, ",");
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
                // skip comment
                if (input[index + 1] == '/') {
                    get_chars_until(input, &index, '\n');
                    break;
                }
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
            EXIT_ERRORF("lexer: unknown char = \"%c\", code = (%d)\n", c, c)
        }

        index++;
    }

    token_tail = token_add_next(token_tail, TokenTypeEndOfFile, "");

    return token_head;
}
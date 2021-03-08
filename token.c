#include <stdlib.h>
#include <stdio.h>

#include "token.h"

token_t *new_token(unsigned int type, char *value) {
    token_t *token = malloc(sizeof(token_t));
    token->type = type;
    token->value = value;
    token->prev = NULL;
    token->next = NULL;
    return token;
}

token_t *token_add_next(token_t *prev, int type, char *value) {
    token_t *next = new_token(type, value);
    prev->next = next;
    next->prev = prev;
    return next;
}

void print_tokens(token_t *head) {
    token_t *current_token = head;

    while (current_token) {
        printf("%s\n", token_to_string(current_token));
        current_token = current_token->next;
    }
}


char *token_name(unsigned int type) {
    switch (type) {
        case TokenTypeEndOfFile:
            return "TokenTypeEndOfFile";
        case TokenTypeStart:
            return "TokenTypeStart";
        case TokenTypeKeyword:
            return "TokenTypeKeyword";
        case TokenTypeDataType:
            return "TokenTypeDataType";
        case TokenTypeStringLiteral:
            return "TokenTypeStringLiteral";
        case TokenTypeIntegerLiteral:
            return "TokenTypeIntegerLiteral";
        case TokenTypeSemiColon:
            return "TokenTypeSemiColon";
        case TokenTypeComma:
            return "TokenTypeComma";
        case TokenTypeEquals:
            return "TokenTypeEquals";
        case TokenTypeMinus:
            return "TokenTypeMinus";
        case TokenTypePlus:
            return "TokenTypePlus";
        case TokenTypeAsterisk:
            return "TokenTypeAsterisk";
        case TokenTypeForwardSlash:
            return "TokenTypeForwardSlash";
        case TokenTypeIdentifier:
            return "TokenTypeIdentifier";
        case TokenTypeLeftParen:
            return "TokenTypeLeftParen";
        case TokenTypeRightParen:
            return "TokenTypeRightParen";
        case TokenTypeLeftBrace:
            return "TokenTypeLeftBrace";
        case TokenTypeRightBrace:
            return "TokenTypeRightBrace";
        case TokenTypeAngleBracketLeft:
            return "TokenTypeAngleBracketLeft";
        case TokenTypeAngleBracketRight:
            return "TokenTypeAngleBracketRight";
        default:
            return "Unknown";
    }
}

char *token_to_string(token_t *token) {
    char *s = malloc(sizeof(char) * 120);
    sprintf(s, "{ type: %s, value: \"%s\" }", token_name(token->type), token->value);
    return s;
}

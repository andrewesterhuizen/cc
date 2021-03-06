#ifndef TOKEN_H
#define TOKEN_H

enum {
    TokenTypeEndOfFile,
    TokenTypeStart,
    TokenTypeKeyword,
    TokenTypeDataType,
    TokenTypeIdentifier,
    TokenTypeIntegerLiteral,
    TokenTypeStringLiteral,
    TokenTypeSemiColon,
    TokenTypeEquals,
    TokenTypePlus,
    TokenTypeMinus,
    TokenTypeAsterisk,
    TokenTypeForwardSlash,
    TokenTypeLeftParen,
    TokenTypeRightParen,
    TokenTypeLeftBrace,
    TokenTypeRightBrace,
    TokenTypeAngleBracketLeft,
    TokenTypeAngleBracketRight
};

typedef struct token {
    unsigned int type;
    char *value;
    struct token_t *next;
} token_t;

token_t *new_token(unsigned int type, char *value);

token_t* token_add_next(token_t *prev, int type, char *value);

void print_tokens(token_t *head);

char *token_name(unsigned int type);

char *token_to_string(token_t *token);

#endif //TOKEN_H
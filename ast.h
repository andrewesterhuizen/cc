#ifndef AST_H
#define AST_H

#include "token.h"

enum {
    AstNodeTypeProgram,
    AstNodeTypeExpression,
    AstNodeTypeStatement,
    AstNodeTypeEmptyStatement,
    AstNodeTypeDeclarationStatement,
    AstNodeIntegerLiteralExpression
};

enum {
    DataTypeInt
};

struct ast_node_expression {
    unsigned int type;
    union {
        unsigned int value_int;
    };
} typedef ast_node_expression_t;

struct declaration {
    unsigned int data_type;
    char *identifier;
    ast_node_expression_t* value;
};

struct ast_node_statement {
    unsigned int type;
    struct ast_node_statement *next;

    union {
        struct declaration declaration;
    };

} typedef ast_node_statement_t;

struct ast_node {
    unsigned int type;
    struct ast_node_statement *statements;
} typedef ast_node_t;

ast_node_t *get_ast(token_t *tokens);

void print_statement(ast_node_statement_t *s);

void print_ast(ast_node_t *node);

char*  data_type_to_string(unsigned int type);

#endif //AST_H

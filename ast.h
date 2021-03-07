#ifndef AST_H
#define AST_H

#include "token.h"

enum {
    AstNodeTypeProgram,
    AstNodeTypeExpression,
    AstNodeTypeStatement,
    AstNodeTypeEmptyStatement,
    AstNodeTypeBlockStatement,
    AstNodeTypeExpressionStatement,
    AstNodeTypeDeclarationStatement,
    AstNodeTypeFunctionDeclarationStatement,
    AstNodeIntegerLiteralExpression,
    AstNodeFunctionCallExpression,
    AstNodeTypeIdentifierExpression,
};

enum {
    DataTypeInt = 1
};

struct data_type {
    unsigned int type;
    int is_pointer;
} typedef data_type_t;

struct function_call_expression {
    char *identifier;
    struct ast_node_expression *arguments;
};

struct ast_node_expression {
    unsigned int type;
    union {
        struct function_call_expression function_call_expression;
        unsigned int value_int;
        char *identifier;
    };
    struct ast_node_expression *next;
} typedef ast_node_expression_t;

struct declaration {
    data_type_t data_type;
    char *identifier;
    ast_node_expression_t *value;
};

struct function_declaration_parameter {
    data_type_t data_type;
    char *identifier;
    struct function_declaration_parameter *next;
} typedef function_declaration_parameter_t;

function_declaration_parameter_t *new_function_declaration_parameter(data_type_t data_type, char *identifier);

struct function_declaration {
    char *identifier;
    data_type_t return_type;
    function_declaration_parameter_t *parameters;
    struct ast_node_statement *body;
};

struct block {
    struct ast_node_statement *statements;
};


struct ast_node_statement {
    unsigned int type;
    struct ast_node_statement *next;

    union {
        struct declaration declaration;
        struct function_declaration function_declaration;
        struct block block;
        ast_node_expression_t *expression;
    };

} typedef ast_node_statement_t;

struct ast_node {
    unsigned int type;
    struct ast_node_statement *statements;
} typedef ast_node_t;

void print_statement(ast_node_statement_t *s);

char *ast_to_string(ast_node_t *node);

char *data_type_to_string(data_type_t);

char *ast_node_type_to_string(unsigned int type);

char *function_declaration_parameter_to_string(function_declaration_parameter_t *declaration);

ast_node_expression_t *new_ast_expression_node(unsigned int type);

ast_node_statement_t *new_ast_statement_node(unsigned int type);

unsigned int get_data_type(char *type_name);

#endif //AST_H

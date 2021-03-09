#ifndef AST_H
#define AST_H

#include "token.h"
#include "string_builder.h"

enum {
    AstNodeTypeProgram,
    AstNodeTypeExpression,
    AstNodeTypeStatement,
    AstNodeTypeEmptyStatement,
    AstNodeTypeBlockStatement,
    AstNodeTypeExpressionStatement,
    AstNodeTypeDeclarationStatement,
    AstNodeTypeFunctionDeclarationStatement,
    AstNodeTypeReturnStatement,
    AstNodeIntegerLiteralExpression,
    AstNodeStringLiteralExpression,
    AstNodeFunctionCallExpression,
    AstNodeTypeIdentifierExpression,
    AstNodeBinaryExpression
};

enum {
    DataTypeInt = 1,
    DataTypeChar,
    DataTypeVoid
};

struct data_type {
    unsigned int type;
    int is_pointer;
} typedef data_type_t;

// expressions
struct function_call_expression {
    char *identifier;
    struct ast_node_expression *arguments;
};

struct binary_expression {
    char *operator;
    struct ast_node_expression *left;
    struct ast_node_expression *right;
};

struct ast_node_expression {
    unsigned int type;
    union {
        struct function_call_expression function_call_expression;
        unsigned int value_int;
        char *value_string;
        char *identifier; // TODO: change this to just use value_string
        struct binary_expression binary_expression;
    };
    struct ast_node_expression *next;
} typedef ast_node_expression_t;

// statements
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

ast_node_statement_t *new_ast_statement_node(unsigned int type);

ast_node_expression_t *new_ast_expression_node(unsigned int type);

function_declaration_parameter_t *new_function_declaration_parameter(data_type_t data_type, char *identifier);

unsigned int get_data_type(char *type_name);

#endif //AST_H

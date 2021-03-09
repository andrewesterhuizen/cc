#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "ast.h"
#include "string_builder.h"
#include "defines.h"
#include "ast_to_string.h"

ast_node_statement_t *new_ast_statement_node(unsigned int type) {
    DEBUGF("new_ast_statement_node: type = %s\n", ast_node_type_to_string(type));

    ast_node_statement_t *s = malloc(sizeof(ast_node_statement_t));
    s->type = type;
    s->next = NULL;
    return s;
}

ast_node_expression_t *new_ast_expression_node(unsigned int type) {
    DEBUGF("new_ast_expression_node: type = %s\n", ast_node_type_to_string(type));

    ast_node_expression_t *s = malloc(sizeof(ast_node_expression_t));
    s->type = type;
    s->next = NULL;
    return s;
}

unsigned int get_data_type(char *type_name) {
    DEBUGF("parser: get_data_type: %s\n", type_name);

    if (strcmp(type_name, "int") == 0) {
        return DataTypeInt;
    }

    if (strcmp(type_name, "char") == 0) {
        return DataTypeChar;
    }

    if (strcmp(type_name, "void") == 0) {
        return DataTypeVoid;
    }

    EXIT_ERRORF("parser: get_data_type: no case defined for data type \"%s\"\n", type_name);
}

function_declaration_parameter_t *new_function_declaration_parameter(data_type_t data_type, char *identifier) {
    function_declaration_parameter_t *p = malloc(sizeof(function_declaration_parameter_t));
    p->identifier = identifier;
    p->data_type.type = data_type.type;
    p->data_type.is_pointer = data_type.is_pointer;
    p->next = NULL;
    return p;
}

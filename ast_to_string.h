#ifndef AST_TO_STRING_H
#define AST_TO_STRING_H

#include "ast.h"
#include "string_builder.h"

struct ast_printer {
    string_builder_t *sb;
    int indent;
} typedef ast_printer_t;

ast_printer_t *new_ast_printer();

char *ast_to_string(ast_node_t *node);

char *ast_node_type_to_string(unsigned int type);

char *data_type_to_string(data_type_t);


#endif //AST_TO_STRING_H

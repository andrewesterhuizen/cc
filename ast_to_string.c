#include "ast_to_string.h"
#include "defines.h"


void ast_printer_do_indent(ast_printer_t *p) {
    for (int i = 0; i < p->indent; i++) {
        string_builder_append(p->sb, " ");
    }
}

void ast_printer_increase_indent(ast_printer_t *p) {
    p->indent += 2;
}

void ast_printer_decrease_indent(ast_printer_t *p) {
    p->indent -= 2;

    if(p->indent < 0) {
        EXIT_ERROR("indent < 0\n")
    }
}

void ast_printer_append(ast_printer_t *p, char *s) {
    ast_printer_do_indent(p);
    string_builder_append(p->sb, s);
}

void ast_printer_append_no_indent(ast_printer_t *p, char *s) {
    string_builder_append(p->sb, s);
}


void expression_to_string(ast_printer_t *p, ast_node_expression_t *expression) {
    DEBUGF("ast_to_string: expression_to_string: type = %s\n", ast_node_type_to_string(expression->type));

    switch (expression->type) {
        case AstNodeIntegerLiteralExpression:
            ast_printer_append_no_indent(p, "integer_literal_expression {\n");
            ast_printer_increase_indent(p);

            ast_printer_append(p, "value: ");
            char int_str[64];
            sprintf(int_str, "%d", expression->value_int);
            ast_printer_append_no_indent(p, int_str);
            ast_printer_append_no_indent(p, ",\n");

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "}");
            break;
        case AstNodeStringLiteralExpression:
            ast_printer_append_no_indent(p, "string_literal_expression {\n");
            ast_printer_increase_indent(p);

            ast_printer_append(p, "value: ");
            ast_printer_append_no_indent(p, expression->value_string);
            ast_printer_append_no_indent(p, ",\n");

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "}");

            break;
        case AstNodeTypeIdentifierExpression:
            ast_printer_append_no_indent(p, "identifier_expression {\n");
            ast_printer_increase_indent(p);

            ast_printer_append(p, "value: ");
            ast_printer_append_no_indent(p, expression->identifier);
            ast_printer_append_no_indent(p, ",\n");


            ast_printer_decrease_indent(p);
            ast_printer_append(p, "}");

            break;
        case AstNodeFunctionCallExpression: {
            ast_printer_append_no_indent(p, "function_call_expression {\n");
            ast_printer_increase_indent(p);

            ast_printer_append(p, "identifier: ");
            ast_printer_append_no_indent(p, "\"");
            ast_printer_append_no_indent(p, expression->function_call_expression.identifier);
            ast_printer_append_no_indent(p, "\"");
            ast_printer_append_no_indent(p, ",\n");


            ast_printer_append(p, "args: [\n");
            ast_printer_increase_indent(p);

            ast_node_expression_t *args = expression->function_call_expression.arguments;

            if (args != NULL) {
                for (ast_node_expression_t *e = args; e != NULL; e = e->next) {
                    ast_printer_do_indent(p);
                    expression_to_string(p, e);
                    ast_printer_append_no_indent(p, ",\n");
                }
            }

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "]\n");

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "}");
            break;
        }

        case AstNodeBinaryExpression: {
            ast_printer_append_no_indent(p, "binary_expression {\n");
            ast_printer_increase_indent(p);

            ast_printer_append(p, "operator: ");
            ast_printer_append_no_indent(p, expression->binary_expression.operator);
            ast_printer_append_no_indent(p, ",\n");

            ast_printer_append(p, "left: ");
            expression_to_string(p, expression->binary_expression.left);
            ast_printer_append_no_indent(p, ",\n");

            ast_printer_append(p, "right: ");
            expression_to_string(p, expression->binary_expression.right);
            ast_printer_append_no_indent(p, ",\n");

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "}");
            break;
        }

        default:
        EXIT_ERRORF("ast_to_string: expression_to_string: no case defined for expression type %d\n", expression->type);
    }
}

void function_declaration_parameter_to_string(ast_printer_t *p, function_declaration_parameter_t *declaration) {
    DEBUGF("ast_to_string: function_declaration_parameter_to_string: name = %s\n", declaration->identifier);

    ast_printer_append(p, "parameter { identifier: ");
    ast_printer_append_no_indent(p, declaration->identifier);
    ast_printer_append_no_indent(p, ", ");

    ast_printer_append_no_indent(p, "data_type: ");
    ast_printer_append_no_indent(p, data_type_to_string(declaration->data_type));
    ast_printer_append_no_indent(p, " }");
}

char *statement_to_string(ast_printer_t *p, ast_node_statement_t *s) {
    DEBUGF("ast_to_string: statement_to_string: type = %s\n", ast_node_type_to_string(s->type));

    char *out = malloc(sizeof(char) * 2056);

    switch (s->type) {
        case AstNodeTypeEmptyStatement:
            break;
        case AstNodeTypeDeclarationStatement: {


            ast_printer_append(p, "declaration_statement {\n");
            ast_printer_increase_indent(p);

            ast_printer_append(p, "identifier: ");
            ast_printer_append_no_indent(p, s->declaration.identifier);
            ast_printer_append(p, "\n");

            ast_printer_append(p, "data_type: ");
            ast_printer_append_no_indent(p, data_type_to_string(s->declaration.data_type));
            ast_printer_append(p, "\n");

            ast_printer_append(p, "value: ");

            if (s->declaration.value != NULL) {
                expression_to_string(p, s->declaration.value);
            } else {
                ast_printer_append_no_indent(p, "NULL");
            }

            ast_printer_append(p, "\n}\n");
            ast_printer_decrease_indent(p);

            ast_printer_append(p, "}");
            ast_printer_append(p, "\n");
            break;
        }

        case AstNodeTypeFunctionDeclarationStatement: {
            ast_printer_append(p, "function_declaration_statement {\n");
            ast_printer_increase_indent(p);

            ast_printer_append(p, "identifier: ");
            ast_printer_append_no_indent(p, s->function_declaration.identifier);
            ast_printer_append(p, "\n");

            ast_printer_append(p, "parameters: [\n");
            ast_printer_increase_indent(p);

            function_declaration_parameter_t *current_parameter = s->function_declaration.parameters;
            while (current_parameter != NULL) {
                function_declaration_parameter_to_string(p, current_parameter);
                ast_printer_append_no_indent(p, ",\n");
                current_parameter = current_parameter->next;
            }

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "]\n");

            ast_printer_append(p, "return_type: ");
            ast_printer_append_no_indent(p, data_type_to_string(s->function_declaration.return_type));
            ast_printer_append_no_indent(p, "\n");

            ast_printer_append(p, "value: ");
            statement_to_string(p, s->function_declaration.body);
            ast_printer_append_no_indent(p, "\n");

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "}\n");

            break;
        }

        case AstNodeTypeBlockStatement: {
            ast_printer_append_no_indent(p, "block_statement {\n");
            ast_printer_increase_indent(p);

            ast_printer_append(p, "body: [\n");
            ast_printer_increase_indent(p);

            ast_node_statement_t *current_statement = s->block.statements;
            while (current_statement != NULL) {
                statement_to_string(p, current_statement);
                ast_printer_append_no_indent(p, ",\n");
                current_statement = current_statement->next;
            }

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "]\n");

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "}");
            break;
        }

        case AstNodeTypeExpressionStatement: {
            ast_printer_append(p, "expression_statement {\n");
            ast_printer_increase_indent(p);
            ast_printer_append(p, "expression: ");
            expression_to_string(p, s->expression);
            ast_printer_append_no_indent(p, "\n");
            ast_printer_decrease_indent(p);
            ast_printer_append(p, "}");
            break;
        }

        case AstNodeTypeReturnStatement: {
            ast_printer_append(p, "return_statement {\n");
            ast_printer_append(p, "expression: ");
            expression_to_string(p, s->expression);
            ast_printer_append(p, "}");
            break;
        }

        default:
        EXIT_ERRORF("ast_to_string: statement_to_string: no case defined for statement type %d\n", s->type);
    }

    return out;
}


ast_printer_t *new_ast_printer() {
    ast_printer_t *p = malloc(sizeof(ast_printer_t));
    p->sb = string_builder_new();
    p->indent = 0;

    return p;
}


char *ast_to_string(ast_node_t *node) {
    DEBUGF("ast_to_string: type = %s\n", ast_node_type_to_string(node->type));

    ast_printer_t *p = new_ast_printer();

    switch (node->type) {
        case AstNodeTypeProgram: {
            ast_printer_append(p, "program {\n");
            ast_printer_increase_indent(p);

            ast_printer_append(p, "statements: [\n");
            ast_printer_increase_indent(p);

            ast_node_statement_t *s = node->statements;
            while (s != NULL) {
                statement_to_string(p, s);
                s = s->next;
            }

            ast_printer_decrease_indent(p);
            ast_printer_append(p, "]\n");
            ast_printer_decrease_indent(p);
            ast_printer_append(p, "}\n");

            break;
            default:
            EXIT_ERRORF("ast_to_string: no case defined for node type %d\n", node->type);
        }
    }

    char *out = string_builder_get_string_copy(p->sb);
    string_builder_destroy(p->sb);


    return out;
}

char *data_type_to_string(data_type_t data_type) {
    string_builder_t *sb = string_builder_new();

    switch (data_type.type) {
        case DataTypeInt:
            string_builder_append(sb, "int");
            break;
        case DataTypeChar:
            string_builder_append(sb, "char");
            break;
        case DataTypeVoid:
            string_builder_append(sb, "void");
            break;
        default:
            EXIT_ERRORF("ast_to_string: data_type_to_string: no case defined for data type %d\n", data_type.type)
    }

    if (data_type.is_pointer) {
        string_builder_append(sb, "*");
    }

    char *out = string_builder_get_string_copy(sb);
    string_builder_destroy(sb);

    return out;
}

char *ast_node_type_to_string(unsigned int type) {
    switch (type) {
        case AstNodeTypeProgram:
            return "AstNodeTypeProgram";
        case AstNodeTypeExpression:
            return "AstNodeTypeExpression";
        case AstNodeTypeStatement:
            return "AstNodeTypeStatement";
        case AstNodeTypeEmptyStatement:
            return "AstNodeTypeEmptyStatement";
        case AstNodeTypeBlockStatement:
            return "AstNodeTypeBlockStatement";
        case AstNodeTypeExpressionStatement:
            return "AstNodeTypeExpressionStatement";
        case AstNodeTypeDeclarationStatement:
            return "AstNodeTypeDeclarationStatement";
        case AstNodeTypeFunctionDeclarationStatement:
            return "AstNodeTypeFunctionDeclarationStatement";
        case AstNodeTypeReturnStatement:
            return "AstNodeTypeReturnStatement";
        case AstNodeIntegerLiteralExpression:
            return "AstNodeIntegerLiteralExpression";
        case AstNodeStringLiteralExpression:
            return "AstNodeStringLiteralExpression";
        case AstNodeFunctionCallExpression:
            return "AstNodeFunctionCallExpression";
        case AstNodeTypeIdentifierExpression:
            return "AstNodeTypeIdentifierExpression";
        case AstNodeBinaryExpression:
            return "AstNodeBinaryExpression";
        default:
            return "Unknown";
    }
}

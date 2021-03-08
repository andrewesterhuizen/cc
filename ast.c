#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "ast.h"
#include "string_builder.h"
#include "defines.h"

char *expression_to_string(ast_node_expression_t *expression) {
    DEBUGF("expression_to_string: type = %s\n", ast_node_type_to_string(expression->type));

    char *out = malloc(sizeof(char) * 256);

    switch (expression->type) {
        case AstNodeIntegerLiteralExpression:
            sprintf(out, "integer_literal_expression { value: %d }", expression->value_int);
            break;
        case AstNodeTypeIdentifierExpression:
            sprintf(out, "identifier_expression { identifier: %s }", expression->identifier);
            break;
        case AstNodeFunctionCallExpression: {
            string_builder_t *sb = string_builder_new();

            string_builder_append(sb, "[");

            ast_node_expression_t *args = expression->function_call_expression.arguments;

            if (args != NULL) {
                for (ast_node_expression_t *e = args; e != NULL; e = e->next) {
                    string_builder_append(sb, expression_to_string(e));
                    string_builder_append(sb, ", ");
                }
            }

            string_builder_append(sb, "]");

            char *args_string = string_builder_get_string_copy(sb);
            string_builder_destroy(sb);

            sprintf(out, "function_call_expression {\n  identifier: %s,\n  args: %s }",
                    expression->function_call_expression.identifier,
                    args_string);
            break;
        }

        case AstNodeBinaryExpression: {
            sprintf(out, "binary_expression {\n  operator: %s,\n  left: %s,\n  right: %s }",
                    expression->binary_expression.operator,
                    expression_to_string(expression->binary_expression.left),
                    expression_to_string(expression->binary_expression.right));
            break;
        }

        default:
        EXIT_ERRORF("expression_to_string: no case defined for expression type %d\n", expression->type);
    }

    return out;
}

char *statement_to_string(ast_node_statement_t *s) {
    DEBUGF("statement_to_string: type = %s\n", ast_node_type_to_string(s->type));

    char *out = malloc(sizeof(char) * 2056);

    switch (s->type) {
        case AstNodeTypeEmptyStatement:
            break;
        case AstNodeTypeDeclarationStatement: {
            char *val = "NULL";
            if (s->declaration.value != NULL) {
                val = expression_to_string(s->declaration.value);
            }

            sprintf(out, "declaration_statement {\n  identifier: %s,\n  date_type: %s,\n  value: %s \n}",
                    s->declaration.identifier,
                    data_type_to_string(s->declaration.data_type),
                    val);
            break;
        }

        case AstNodeTypeFunctionDeclarationStatement: {
            string_builder_t *sb = string_builder_new();
            string_builder_append(sb, "[");

            function_declaration_parameter_t *current_parameter = s->function_declaration.parameters;
            while (current_parameter != NULL) {
                string_builder_append(sb, function_declaration_parameter_to_string(current_parameter));
                string_builder_append(sb, ",");
                current_parameter = current_parameter->next;
            }

            string_builder_append(sb, "]");

            char *params_string = string_builder_get_string_copy(sb);
            string_builder_destroy(sb);


            sprintf(out,
                    "function_declaration_statement {\n  identifier: %s,\n  parameters: %s,\n  return_type: %s,\n  value: %s \n}",
                    s->function_declaration.identifier,
                    params_string,
                    data_type_to_string(s->function_declaration.return_type),
                    statement_to_string(s->function_declaration.body));
            break;
        }

        case AstNodeTypeBlockStatement: {
            string_builder_t *sb = string_builder_new();
            string_builder_append(sb, "[");

            ast_node_statement_t *current_statement = s->block.statements;
            while (current_statement != NULL) {
                string_builder_append(sb, statement_to_string(current_statement));
                string_builder_append(sb, ",");
                current_statement = current_statement->next;
            }

            string_builder_append(sb, "]");

            sprintf(out, "block_statement {\n  body: %s \n}", string_builder_get_string_copy(sb));
            string_builder_destroy(sb);

            break;
        }

        case AstNodeTypeExpressionStatement: {
            sprintf(out, "expression_statement {\n  expression: %s\n}", expression_to_string(s->expression));
            break;
        }

        case AstNodeTypeReturnStatement: {
            sprintf(out, "return_statement {\n  expression: %s\n}", expression_to_string(s->expression));
            break;
        }

        default:
        EXIT_ERRORF("statement_to_string: no case defined for statement type %d\n", s->type);
    }

    return out;
}

char *function_declaration_parameter_to_string(function_declaration_parameter_t *declaration) {
    DEBUGF("function_declaration_parameter_to_string: name = %s\n", declaration->identifier);

    char *out = malloc(sizeof(char) * 256);
    sprintf(out, "parameter { identifier: %s, data_type: %s }", declaration->identifier,
            data_type_to_string(declaration->data_type));
    return out;
}

char *ast_to_string(ast_node_t *node) {
    DEBUGF("print_ast: type = %s\n", ast_node_type_to_string(node->type));

    string_builder_t *sb = string_builder_new();

    switch (node->type) {
        case AstNodeTypeProgram: {
            string_builder_append(sb, "program {\n statements: [\n");
            ast_node_statement_t *s = node->statements;
            while (s != NULL) {
                string_builder_append(sb, statement_to_string(s));
                s = s->next;
            }
            string_builder_append(sb, "\n]}\n");
            break;
            default:
            EXIT_ERRORF("print_ast: no case defined for node type %d\n", node->type);
        }
    }

    char *out = string_builder_get_string_copy(sb);
    string_builder_destroy(sb);

    return out;
}

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
    DEBUGF("get_data_type: %s\n", type_name);

    if (strcmp(type_name, "int") == 0) {
        return DataTypeInt;
    }

    EXIT_ERRORF("get_data_type: no case defined for data type \"%s\"\n", type_name);
}

char *data_type_to_string(data_type_t type) {
    string_builder_t *sb = string_builder_new();

    switch (type.type) {
        case DataTypeInt:
            string_builder_append(sb, "Integer");
            break;
        default:
            string_builder_append(sb, "Unknown");
    }

    if (type.is_pointer) {
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

function_declaration_parameter_t *new_function_declaration_parameter(data_type_t data_type, char *identifier) {
    function_declaration_parameter_t *p = malloc(sizeof(function_declaration_parameter_t));
    p->identifier = identifier;
    p->data_type.type = data_type.type;
    p->data_type.is_pointer = data_type.is_pointer;
    p->next = NULL;
    return p;
}

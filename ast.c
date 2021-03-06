#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "ast.h"
#include "lexer.h"

#include "defines.h"

char *expression_to_string(ast_node_expression_t *e) {
    char *s = malloc(sizeof(char) * 120);

    switch (e->type) {
        case AstNodeIntegerLiteralExpression:
            sprintf(s, "integer_literal_expression { value: %d }", e->value_int);
            break;
        default:
        EXIT_ERRORF("expression_to_string: no case defined for expression type %d\n", e->type);
    }

    return s;
}

void print_statement(ast_node_statement_t *s) {
    switch (s->type) {
        case AstNodeTypeEmptyStatement:
            break;
        case AstNodeTypeDeclarationStatement:
            if (s->declaration.value != NULL) {
                printf("declaration_statement { date_type: %s, identifier: %s, value: %s }",
                       data_type_to_string(s->declaration.data_type),
                       s->declaration.identifier,
                       expression_to_string(s->declaration.value));
            } else {
                printf("declaration_statement { date_type: %s, identifier: %s }",
                       data_type_to_string(s->declaration.data_type),
                       s->declaration.identifier);
            }

            break;
        default:
        EXIT_ERRORF("print_statement: no case defined for statement type %d\n", s->type);
    }
}

void print_ast(ast_node_t *node) {
    switch (node->type) {
        case AstNodeTypeProgram: {
            ast_node_statement_t *s = node->statements;
            while (s != NULL) {
                print_statement(s);
                s = s->next;
            }
            break;
            default:
            EXIT_ERRORF("print_ast: no case defined for node type %d\n", node->type);
        }
    }
}

ast_node_statement_t *new_ast_statement_node(unsigned int type) {
    ast_node_statement_t *s = malloc(sizeof(ast_node_statement_t));
    s->type = type;
    s->next = NULL;
    return s;
}

ast_node_expression_t *new_ast_expression_node(unsigned int type) {
    ast_node_statement_t *s = malloc(sizeof(ast_node_expression_t));
    s->type = type;
    s->next = NULL;
    return s;
}

void expect_type(token_t *token, unsigned int type) {
    if (token->type != type) {
        EXIT_ERRORF("expected type %s and got %s for token with value \"%s\"\n",
                    token_name(type),
                    token_name(token->type),
                    token->value);
    }
}

unsigned int get_data_type(char *type_name) {
    if (strcmp(type_name, "int") == 0) {
        return DataTypeInt;
    }

    EXIT_ERRORF("get_data_type: no case defined for data type \"%s\"\n", type_name);
}

ast_node_expression_t *parse_expression(token_t *tokens, token_t *current_token) {
    switch (current_token->type) {
        case TokenTypeIntegerLiteral: {
            ast_node_expression_t *e = new_ast_expression_node(AstNodeIntegerLiteralExpression);
            e->value_int = atoi(current_token->value);
            return e;
        }
        default:
        EXIT_ERRORF("parse_expression: unexpected token: %s\n", token_to_string(current_token));
    }
}

ast_node_t *get_ast(token_t *tokens) {
    ast_node_t *program = malloc(sizeof(ast_node_t));
    program->type = AstNodeTypeProgram;
    program->statements = new_ast_statement_node(AstNodeTypeEmptyStatement);
    ast_node_statement_t *statements_tail = program->statements;

    token_t *current_token = tokens;
    while (current_token) {
        switch (current_token->type) {
            case TokenTypeStart:
            case TokenTypeEndOfFile:
                break;
            case TokenTypeDataType: {
                ast_node_statement_t *s = new_ast_statement_node(AstNodeTypeDeclarationStatement);

                s->declaration.data_type = get_data_type(current_token->value);

                current_token = current_token->next;
                expect_type(current_token, TokenTypeIdentifier);
                s->declaration.identifier = strdup(current_token->value);

                current_token = current_token->next;
                if (current_token->type == TokenTypeEquals) {
                    current_token = current_token->next;
                    expect_type(current_token, TokenTypeIntegerLiteral);

                    ast_node_expression_t *e = parse_expression(tokens, current_token);
                    s->declaration.value = e;
                }

                current_token = current_token->next;
                expect_type(current_token, TokenTypeSemiColon);

                statements_tail->next = s;
                statements_tail = s;

                break;
            }
            default:
            EXIT_ERRORF("get_ast: unexpected token: %s\n", token_to_string(current_token));
        }

        current_token = current_token->next;
    }
    return program;
}

char *data_type_to_string(unsigned int type) {
    switch (type) {
        case DataTypeInt:
            return "Integer";
        default:
            return "Unknown";
    }
}

#include "ast.h"

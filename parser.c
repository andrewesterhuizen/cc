#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "string_builder.h"
#include "defines.h"

void parser_expect(parser_t *parser, unsigned int type) {
    DEBUGF("parser_expect: %s == %s\n", token_name(parser->current_token->type), token_name(type))

    if (parser->current_token->type != type) {
        EXIT_ERRORF("expected type %s and got a %s with value \"%s\"\n",
                    token_name(type),
                    token_name(parser->current_token->type),
                    parser->current_token->value);
    }
}

ast_node_expression_t *parser_parse_function_call_expression(parser_t *parser, token_t *identifier_token) {
    ast_node_expression_t *expression = new_ast_expression_node(AstNodeFunctionCallExpression);
    expression->function_call_expression.identifier = identifier_token->value;
    expression->function_call_expression.arguments = NULL;

    parser_next_token(parser);

    ast_node_expression_t *last_arg = NULL;

    // arguments
    while (parser->current_token->type != TokenTypeRightParen) {
        ast_node_expression_t *arg = parser_parse_expression(parser);

        if (expression->function_call_expression.arguments == NULL) {
            expression->function_call_expression.arguments = arg;
            last_arg = arg;
        } else {
            last_arg->next = arg;
        }

        parser_next_token(parser);
        if (parser->current_token->type == TokenTypeComma) {
            parser_next_token(parser);
        }
    }

    parser_expect(parser, TokenTypeRightParen);
    parser_next_token(parser);

    return expression;
}

ast_node_expression_t *parser_parse_expression(parser_t *parser) {
    DEBUGF("parser_parse_expression: type = %s\n", token_name(parser->current_token->type))

    switch (parser->current_token->type) {
        case TokenTypeIntegerLiteral: {
            ast_node_expression_t *e = new_ast_expression_node(AstNodeIntegerLiteralExpression);
            e->value_int = atoi(parser->current_token->value);

            parser_next_token(parser);
            if (parser->current_token->type == TokenTypeSemiColon ||
                parser->current_token->type == TokenTypeRightParen ||
                parser->current_token->type == TokenTypeComma) {
                parser_prev_token(parser);
                return e;
            }

            token_t *operator_token = parser->current_token;
            parser_next_token(parser);

            ast_node_expression_t *right = parser_parse_expression(parser);

            ast_node_expression_t *binary_expression = new_ast_expression_node(AstNodeBinaryExpression);

            binary_expression->binary_expression.operator = operator_token->value;
            binary_expression->binary_expression.left = e;
            binary_expression->binary_expression.right = right;

            return binary_expression;
        }
        case TokenTypeStringLiteral: {
            ast_node_expression_t *e = new_ast_expression_node(AstNodeStringLiteralExpression);
            e->value_string = parser->current_token->value;
            return e;
        }
        case TokenTypeIdentifier: {
            token_t *identifier_token = parser->current_token;
            parser_next_token(parser);

            ast_node_expression_t *e = NULL;

            if (parser->current_token->type == TokenTypeLeftParen) {
                e = parser_parse_function_call_expression(parser, identifier_token);
            } else {
                e = new_ast_expression_node(AstNodeTypeIdentifierExpression);
                e->identifier = identifier_token->value;
            }

            if (parser->current_token->type == TokenTypeSemiColon) {
                parser_prev_token(parser);
                return e;
            }

            token_t *operator_token = parser->current_token;
            parser_next_token(parser);

            ast_node_expression_t *right = parser_parse_expression(parser);

            ast_node_expression_t *binary_expression = new_ast_expression_node(AstNodeBinaryExpression);

            binary_expression->binary_expression.operator = operator_token->value;
            binary_expression->binary_expression.left = e;
            binary_expression->binary_expression.right = right;

            return binary_expression;
        }
        default:
        EXIT_ERRORF("parser_parse_expression: unexpected token: %s\n", token_to_string(parser->current_token));
    }
}

ast_node_statement_t *parser_parse_function_declaration_statement(parser_t *parser,
                                                                  char *identifier,
                                                                  char *data_type_name,
                                                                  int is_pointer) {
    unsigned int data_type = get_data_type(data_type_name);

    DEBUGF("parser_parse_function_declaration_statement: identifier = %s, data_type = %s, is_pointer = %d \n",
           identifier,
           data_type_name,
           is_pointer)


    ast_node_statement_t *s = new_ast_statement_node(AstNodeTypeFunctionDeclarationStatement);

    s->function_declaration.return_type.type = data_type;
    s->function_declaration.return_type.is_pointer = is_pointer;

    s->function_declaration.identifier = identifier;

    // TODO: I think C functions don't actually need typed parameters

    // parameters
    function_declaration_parameter_t *parameters = NULL;
    function_declaration_parameter_t *current_parameter = NULL;

    parser_next_token(parser);
    while (parser->current_token->type != TokenTypeRightParen) {
        parser_expect(parser, TokenTypeDataType);
        token_t *parameter_data_type_token = parser->current_token;

        data_type_t parameter_data_type;
        parameter_data_type.type = get_data_type(parameter_data_type_token->value);
        parameter_data_type.is_pointer = 0;

        parser_next_token(parser);
        if (parser->current_token->type == TokenTypeAsterisk) {
            parser_next_token(parser);
            parameter_data_type.is_pointer = 1;
        }

        parser_expect(parser, TokenTypeIdentifier);
        token_t *identifier = parser->current_token;


        function_declaration_parameter_t *p = new_function_declaration_parameter(
                parameter_data_type,
                identifier->value);

        parser_next_token(parser);

        if (parameters == NULL) {
            parameters = p;
            current_parameter = p;
        } else {
            current_parameter->next = p;
        }

        if (parser->current_token->type == TokenTypeComma) {
            parser_next_token(parser);
        }
    }

    parser_expect(parser, TokenTypeRightParen);

    s->function_declaration.parameters = parameters;

    s->function_declaration.body = parser_parse_block_statement(parser);

    return s;
}

ast_node_statement_t *
parser_parse_declaration_statement(parser_t *parser, char *identifier, char *data_type_name, int is_pointer) {
    unsigned int data_type = get_data_type(data_type_name);

    DEBUGF("parser_parse_declaration_statement: identifier = %s, data_type = %s, is_pointer = %d \n",
           identifier,
           data_type_name,
           is_pointer)

    ast_node_statement_t *s = new_ast_statement_node(AstNodeTypeDeclarationStatement);

    s->declaration.data_type.type = data_type;
    s->declaration.data_type.is_pointer = is_pointer;
    s->declaration.identifier = identifier;

    parser_next_token(parser);

    ast_node_expression_t *e = parser_parse_expression(parser);
    s->declaration.value = e;

    return s;
}


ast_node_statement_t *parser_parse_statements(parser_t *parser) {
    DEBUGF("parser_parse_statements\n")

    ast_node_statement_t *statements = new_ast_statement_node(AstNodeTypeEmptyStatement);
    ast_node_statement_t *current_statement = statements;

    while (parser->current_token) {
        switch (parser->current_token->type) {
            case TokenTypeStart:
                break;
            case TokenTypeEndOfFile:
                return statements->next;
            case TokenTypeRightBrace:
                parser_prev_token(parser);
                return statements->next;
            case TokenTypeDataType: {
                token_t *data_type_token = parser->current_token;
                parser_next_token(parser);

                int is_pointer = 0;
                if (parser->current_token->type == TokenTypeAsterisk) {
                    is_pointer = 1;
                    parser_next_token(parser);
                }

                parser_expect(parser, TokenTypeIdentifier);
                token_t *identifier_token = parser->current_token;

                parser_next_token(parser);

                // TODO: this should be switch instead of if statements
                if (parser->current_token->type == TokenTypeEquals) {
                    // assignment
                    ast_node_statement_t *s = parser_parse_declaration_statement(parser,
                                                                                 identifier_token->value,
                                                                                 data_type_token->value,
                                                                                 is_pointer);
                    current_statement->next = s;
                    current_statement = s;

                    parser_next_token(parser);
                    parser_expect(parser, TokenTypeSemiColon);
                } else if (parser->current_token->type == TokenTypeLeftParen) {
                    // function declaration
                    ast_node_statement_t *s = parser_parse_function_declaration_statement(parser,
                                                                                          identifier_token->value,
                                                                                          data_type_token->value,
                                                                                          is_pointer);

                    current_statement->next = s;
                    current_statement = s;
                }

                break;
            }
            case TokenTypeIdentifier: {
                ast_node_expression_t *expression = parser_parse_expression(parser);

                parser_next_token(parser);
                parser_expect(parser, TokenTypeSemiColon);

                ast_node_statement_t *expression_statement = new_ast_statement_node(AstNodeTypeExpressionStatement);
                expression_statement->expression = expression;

                current_statement->next = expression_statement;
                current_statement = expression_statement;
                break;
            }

            case TokenTypeKeyword: {
                if (strcmp(parser->current_token->value, "return") == 0) {
                    parser_next_token(parser);
                    ast_node_expression_t *expression = parser_parse_expression(parser);

                    parser_next_token(parser);
                    parser_expect(parser, TokenTypeSemiColon);

                    ast_node_statement_t *return_statement = new_ast_statement_node(AstNodeTypeReturnStatement);
                    return_statement->expression = expression;

                    current_statement->next = return_statement;
                    current_statement = return_statement;
                } else {
                    EXIT_ERRORF("parser_parse_statements: unexpected keyword: %s\n", parser->current_token->value);
                }
                break;
            }

            default:
            EXIT_ERRORF("parser_parse_statements: unexpected token: %s\n", token_to_string(parser->current_token));
        }

        parser_next_token(parser);
    }

    return statements->next;
}

token_t *parser_next_token(parser_t *parser) {
    parser->current_token = parser->current_token->next;
    if (parser->current_token == NULL) {
        // just crash for now
        EXIT_ERROR("parser_next_token: next token is NULL\n");
    }

    DEBUGF("parser_next_token: %s\n", token_to_string(parser->current_token))

    return parser->current_token;
}

token_t *parser_prev_token(parser_t *parser) {
    parser->current_token = parser->current_token->prev;
    if (parser->current_token == NULL) {
        // just crash for now
        EXIT_ERROR("parser_prev_token: prev token is NULL\n");
    }

    DEBUGF("parser_prev_token: %s\n", token_to_string(parser->current_token))

    return parser->current_token;
}

ast_node_statement_t *parser_parse_block_statement(parser_t *parser) {
    DEBUGF("parser_parse_block_statement\n")

    ast_node_statement_t *s = new_ast_statement_node(AstNodeTypeBlockStatement);

    parser_next_token(parser);
    parser_expect(parser, TokenTypeLeftBrace);

    parser_next_token(parser);
    ast_node_statement_t *statements = parser_parse_statements(parser);
    s->block.statements = statements;

    parser_next_token(parser);
    parser_expect(parser, TokenTypeRightBrace);

    return s;
}


ast_node_t *parser_get_ast(token_t *tokens) {
    DEBUGF("parser_get_ast\n")
    parser_t *parser = new_parser(tokens);
    parser->program->statements = parser_parse_statements(parser);
    return parser->program;
}

parser_t *new_parser(token_t *tokens) {
    DEBUGF("new_parser\n")
    parser_t *parser = malloc(sizeof(parser_t));
    parser->tokens = tokens;
    parser->current_token = tokens;
    parser->program = malloc(sizeof(ast_node_t));
    parser->program->type = AstNodeTypeProgram;
    parser->program->statements = NULL;
    return parser;
}
#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

struct parser {
    ast_node_t *program;
    token_t *tokens;
    token_t *current_token;
} typedef parser_t;

parser_t *new_parser(token_t *tokens);

ast_node_t *parser_get_ast(token_t *tokens);

ast_node_statement_t *parser_parse_statements(parser_t *parser);

ast_node_statement_t *parser_parse_block_statement(parser_t *parser);

ast_node_expression_t *parser_parse_expression(parser_t *parser);

token_t *parser_next_token(parser_t *parser);

token_t *parser_prev_token(parser_t *parser);


#endif //PARSER_H

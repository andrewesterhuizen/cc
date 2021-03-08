#include <stdio.h>

#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "parser.h"
#include "defines.h"
#include "preprocessor.h"
#include "hash_table.h"

int main() {
    char *program = "char* x = \"a string\";";

    DEBUGF("*** lexing ***\n");
    token_t *tokens = getTokens(program);
    DEBUGF("\n");

    DEBUGF("*** tokens before preprocessor: ***\n");
    print_tokens(tokens);
    DEBUGF("\n");

    tokens = preprocessor_process(tokens);

    DEBUGF("*** tokens after preprocessor: ***\n");
    print_tokens(tokens);
    DEBUGF("\n");

    DEBUGF("*** parsing ***\n");
    ast_node_t *ast = parser_get_ast(tokens);
    DEBUGF("\n");

    DEBUGF("*** ast_to_string: ***\n");
    char *ast_str = ast_to_string(ast);
    DEBUGF("\n");

    DEBUGF("*** ast: ***\n");
    printf(ast_str);
    DEBUGF("\n");

    return 0;
}

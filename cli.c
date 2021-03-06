#include <stdio.h>

#include "lexer.h"
#include "token.h"
#include "ast.h"

int main() {
    char *program = "int x = 99;\n";


    token_t *tokens = getTokens(program);
    print_tokens(tokens);

    ast_node_t *ast = get_ast(tokens);

    print_ast(ast);

    return 0;
}

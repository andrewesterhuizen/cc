#include <stdio.h>

#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "parser.h"
#include "defines.h"

int main() {
    char *program = "int main() { return 1; };\n";

    DEBUGF("*** lexing ***\n");
    token_t *tokens = getTokens(program);
    DEBUGF("\n");

    DEBUGF("*** tokens: ***\n");
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

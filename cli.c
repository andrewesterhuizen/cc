#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "parser.h"
#include "defines.h"
#include "preprocessor.h"
#include "ast_to_string.h"

int main() {
    int fd = open("/Users/andrewesterhuizen/dev/cc/test/hello.c", O_RDONLY);
    int len = lseek(fd, 0, SEEK_END);
    char *data = mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);

    char *program = data;

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
    printf("%s", ast_str);
    DEBUGF("\n");

    munmap(data, len);

    return 0;
}

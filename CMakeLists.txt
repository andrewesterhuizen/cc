cmake_minimum_required(VERSION 3.17)
project(cc C)

set(CMAKE_C_STANDARD 99)

add_executable(cc cli.c lexer.c token.c string_builder.c ast.c parser.c preprocessor.c hash_table.c ast_to_string.c)
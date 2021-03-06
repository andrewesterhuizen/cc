#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include "stdlib.h"
#include "strings.h"
#include "stdio.h"

struct string_builder {
    char *_buffer;
    int _maxBufferSize;
    int _currentSize;
} typedef string_builder_t;

string_builder_t *string_builder_new();

char *string_builder_get_string(string_builder_t *builder);
char *string_builder_get_string_copy(string_builder_t *builder);
void string_builder_append(string_builder_t *builder, char *str);
void string_builder_append_char(string_builder_t *builder, char c);
void string_builder_destroy(string_builder_t *builder);
void string_builder_reset(string_builder_t *builder);

#endif //STRING_BUILDER_H

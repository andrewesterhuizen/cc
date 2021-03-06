#include "string_builder.h"
#include "strings.h"

string_builder_t *string_builder_new() {
    string_builder_t *builder = malloc(sizeof(string_builder_t));

    int startSize = 50;

    builder->_currentSize = 0;
    builder->_maxBufferSize = startSize;
    builder->_buffer = calloc(startSize, sizeof(char));
    strcpy(builder->_buffer, "");
    return builder;
}

char *string_builder_get_string(string_builder_t *builder) {
    return builder->_buffer;
}

char *string_builder_get_string_copy(string_builder_t *builder) {
    char *copy = malloc(builder->_currentSize * sizeof(char));
    strcpy(copy, builder->_buffer);

    return copy;
}

void _check_and_resize_buffer(string_builder_t *builder, size_t stringSize) {
    int nextSize = builder->_currentSize + stringSize;

    if (nextSize > builder->_maxBufferSize) {
        // resize buffer
        int newSize = (builder->_maxBufferSize * 2) + stringSize;
        printf("resize: %d -> %d\n", builder->_maxBufferSize, newSize);
        builder->_buffer = realloc(builder->_buffer, newSize);
        builder->_maxBufferSize = newSize;
    }
}

void string_builder_append(string_builder_t *builder, char *str) {
    int stringSize = strlen(str);

    _check_and_resize_buffer(builder, stringSize);

    strcat(builder->_buffer, str);
    builder->_currentSize += stringSize;
}

void string_builder_append_char(string_builder_t *builder, char c) {
    // there has to be a better way of doing this
    char *cString = malloc(2 * sizeof(char));
    cString[0] = c;
    cString[1] = '\0';

    string_builder_append(builder, cString);
}

void string_builder_destroy(string_builder_t *builder) {
    free(builder->_buffer);
    free(builder);
}
void string_builder_reset(string_builder_t *builder) {
    builder->_currentSize = 0;
    builder->_buffer[0] = '\0';
}
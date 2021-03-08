#include "preprocessor.h"
#include "stdlib.h"
#include "hash_table.h"
#include "defines.h"

token_t *preprocessor_process(token_t *entry_file_tokens) {
    token_t *current_token = entry_file_tokens;
    token_t *processed_tokens_head = NULL;
    token_t *processed_tokens = NULL;

    hash_table_t *defines_table = hash_table_new();

    while (current_token != NULL) {
        switch (current_token->type) {
            case TokenTypePreprocessorInclude:
                // TODO: handle files
                break;
            case TokenTypePreprocessorDefine:
                hash_table_set(defines_table, current_token->value, current_token->extra_value);
                break;
            case TokenTypeIdentifier: {
                char *value = hash_table_get(defines_table, current_token->value);
                if (value != NULL) {
                    current_token->value = value;
                }

                if (processed_tokens == NULL) {
                    processed_tokens_head = current_token;
                    processed_tokens = current_token;
                } else {
                    processed_tokens->next = current_token;
                    processed_tokens = current_token;                }

                break;
            }
            default:
                if (processed_tokens_head == NULL) {
                    processed_tokens_head = current_token;
                    processed_tokens = current_token;
                } else {
                    processed_tokens->next = current_token;
                    processed_tokens = current_token;
                }
        }

        current_token = current_token->next;
    }

    return processed_tokens_head;
}

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define HASH_TABLE_SIZE 101

struct hash_table_entry {
    char *key;
    char *value;
    struct hash_table_entry *next;
} typedef hash_table_entry_t;

struct hash_table {
    hash_table_entry_t **buckets;
} typedef hash_table_t;

unsigned int hash(unsigned char *str);

hash_table_t *hash_table_new();

hash_table_entry_t *hash_table_entry_new(char *key, char *value);

hash_table_t *hash_table_destroy(hash_table_t *h);

char *hash_table_get(hash_table_t *h, char *value);

void hash_table_set(hash_table_t *h, char *key, char *value);


#endif //HASH_TABLE_H

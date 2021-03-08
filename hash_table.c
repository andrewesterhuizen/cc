#include <stdlib.h>
#include <strings.h>

#include "hash_table.h"

// TODO: replace this
unsigned int hash(unsigned char *key)
{
    unsigned int hash = 0;

    int i = 0;
    int c = key[i];

    while (c)
    {
        c = key[i];
        i++;
        hash += c;
    }

    return hash % HASH_TABLE_SIZE;
}

hash_table_t *hash_table_new()
{
    hash_table_t *h = malloc(sizeof(hash_table_t));
    h->buckets = calloc(HASH_TABLE_SIZE, sizeof(hash_table_entry_t *));
    return h;
};

hash_table_entry_t *hash_table_entry_new(char *key, char *value)
{
    hash_table_entry_t *entry = malloc(sizeof(hash_table_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = NULL;
    return entry;
}

char *hash_table_get(hash_table_t *h, char *key)
{
    unsigned int hash_key = hash(key);

    hash_table_entry_t *bucket = h->buckets[hash_key];
    while (bucket != NULL)
    {
        if (strcmp(key, bucket->key) == 0)
        {
            return bucket->value;
        }

        bucket = bucket->next;
    }

    return NULL;
};

void hash_table_set(hash_table_t *h, char *key, char *value)
{
    hash_table_entry_t *entry = hash_table_get(h, key);
    if (entry != NULL)
    {
        // update existing entry
        entry->value = value;
        return;
    }

    // add new entry
    unsigned int hash_key = hash(key);
    entry = hash_table_entry_new(key, value);

    hash_table_entry_t *bucket = h->buckets[hash_key];

    // if no bucket yet, entry will be first item
    if (bucket == NULL)
    {
        h->buckets[hash_key] = entry;
        return;
    }

    // find last item in bucket
    while (bucket->next != NULL)
    {
        bucket = bucket->next;
    }

    bucket->next = entry;
    return;
}

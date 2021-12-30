// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of words in dictionary
unsigned int word_count = 0;

// Number of buckets in hash table
const unsigned int N = 5000;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int hash_value = hash(word);
    node *cursor = table[hash_value];

    // Traverse the linked list to find word
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
// Source: https://github.com/hathix/cs50-section/blob/master/code/7/sample-hash-functions/good-hash-function.c
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    for (const char *ptr = word; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + tolower(*ptr);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    unsigned int words = 0;
    char word[LENGTH + 1];
    // Read strings from file one at a time
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for each word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }
        strcpy(new_node->word, word);
        new_node->next = NULL;

        unsigned int hash_value = hash(word);
        // Insert new node into hash table at that location
        new_node->next = table[hash_value];
        table[hash_value] = new_node;
        words++;
    }

    word_count = words;
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *tmp;
    node *cursor;

    // For each bucket in hash table
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];

        // Traverse linked list to free up nodes
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }

        // Last node has been freed
        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }
    return false;
}

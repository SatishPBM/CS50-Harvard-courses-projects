// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

int dictsize = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{

    int hashword = hash(word);

    node *wordnode = table[hashword];

    while (wordnode != NULL)
    {

        int i = strcasecmp(wordnode->word, word);

        if (i == 0)
        {
            return true;
        }

        wordnode = wordnode->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash function used is String folding (source: https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/HashFuncExamp.html)

    int word_len = strlen(word);

    int sum = 0, mul = 1;

    for (int i = 0; i < word_len; i++)
    {
        mul = (i % 4 == 0) ? 1 : mul * 256;

        sum = sum + tolower(word[i]) * mul;
    }

    return (int)(abs(sum) % N);

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary file

    FILE *dictfile = fopen(dictionary, "r");

    if (dictfile == NULL)
    {
        printf("Cannot open dictionary.\n");
        return false;
    }

    char dictword[LENGTH + 1];

    while (fscanf(dictfile, "%s", dictword) != EOF)
    {
        node *dictnode = malloc(sizeof(node));

        if (dictnode == NULL)
        {
            printf("malloc failed.\n");
            return false;
        }

        strcpy(dictnode->word, dictword);

        int hash_val = hash(dictword);

        dictnode->next = table[hash_val];

        table[hash_val] = dictnode;

        dictsize = dictsize + 1;
    }

    fclose(dictfile);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictsize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            node *tmp = cursor;

            cursor = cursor->next;

            free(tmp);
        }
    }

    return true;

}

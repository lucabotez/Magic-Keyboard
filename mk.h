// Copyright @lucabotez

#ifndef MK_H
#define MK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "trie.h"

#define WORD_SIZE 256

#define DIE(assertion, call_description)                \
    do {                                              \
        if (assertion) {                              \
            fprintf(stderr, "(%s, %d): ",            \
                    __FILE__, __LINE__);              \
            perror(call_description);                 \
            exit(errno);                              \
        }                                             \
    } while (0)

// function headers
int difference(char *word1, char *word2);
void autocorrect(trie_node_t *node, char *new_word, int length, char *word, int num, int *check);
void autocomplete_1(trie_node_t *node, char *word, int length);
void autocomplete_2(trie_node_t *node, char *prefix, char *shortest, int length, int *first);
void autocomplete_3(trie_node_t *node, char *prefix, char *freq, int *prev_freq, int length);
void autocomplete_prep(trie_t *trie, char *prefix, int num);

#endif // MK_H

// Copyright @lucabotez

#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// trie node structure
typedef struct trie_node_t trie_node_t;
struct trie_node_t {
	int end_of_word;

	int frequency;

	trie_node_t **children;
	int n_children;
};

// trie structure
typedef struct trie_t trie_t;
struct trie_t {
	trie_node_t *root;

	int size;

	int data_size;

	int alphabet_size;
};

// function headers
trie_t *trie_create(int data_size, int alphabet_size);
trie_node_t *trie_create_node(trie_t *trie);

void trie_insert(trie_t *trie, char *key);
void trie_remove(trie_t *trie, char *key);

void trie_free_subtrie(trie_t *trie, trie_node_t *node);
void trie_free(trie_t **p_trie);

#endif // TRIE_H
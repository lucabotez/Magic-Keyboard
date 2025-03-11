// Copyright @lucabotez

#include "trie.h"
#include "mk.h"

trie_node_t *trie_create_node(trie_t *trie)
{
	trie_node_t *node = malloc(sizeof(trie_node_t));
	DIE(!node, "node malloc");

	node->end_of_word = 0;
	node->frequency = 0;
	node->n_children = 0;
	node->children = calloc(trie->alphabet_size, sizeof(trie_node_t *));

	return node;
}

trie_t *trie_create(int data_size, int alphabet_size)
{
	trie_t *trie = malloc(sizeof(trie_t));
	DIE(!trie, "trie malloc");

	trie->size = 0;
	trie->data_size = data_size;
	trie->alphabet_size = alphabet_size;

	trie->root = trie_create_node(trie);
	return trie;
}

void trie_insert(trie_t *trie, char *key)
{
	if (key[0] == '\0') {
		trie->root->end_of_word = 1;
		return;
	}

	trie_node_t *current = trie->root;
	int i = 0;

	while (key[i] != '\0') {
		int letter = key[i] - 'a';

		if (!current->children[letter]) {
			current->children[letter] = trie_create_node(trie);
			current->n_children++;
		}

		current = current->children[letter];
		i++;
	}

	if (current->end_of_word) {
		current->frequency++;
	} else {
		current->end_of_word = 1;
		current->frequency = 1;
	}

	trie->size++;
}

void trie_remove(trie_t *trie, char *key)
{
	if (key[0] == '\0') {
		trie->root->end_of_word = 0;
		trie->size--;
		return;
	}

	trie_node_t *current = trie->root;
	trie_node_t *parent = trie->root;
	int parent_letter = (key[0] - 'a');
	int i = 0;

	while (key[i] != '\0') {
		int letter = key[i] - 'a';

		if (!current->children[letter])
			return;

		if (current->n_children > 1 || current->end_of_word) {
			parent = current;
			parent_letter = letter;
		}

		current = current->children[letter];

		i++;
	}

	current->end_of_word = 0;
	current->frequency = 0;
	if (!current->n_children) {
		trie_free_subtrie(trie, parent->children[parent_letter]);
		parent->children[parent_letter] = NULL;
		parent->n_children--;
	}

	trie->size--;
}

void trie_free_subtrie(trie_t *trie, trie_node_t *node)
{
	if (!node)
		return;

	for (int i = 0; i < trie->alphabet_size && node->n_children; ++i) {
		if (!node->children[i])
			continue;

		trie_free_subtrie(trie, node->children[i]);
		node->n_children--;
		node->children[i] = NULL;
	}

	free(node->children);
	free(node);
}

void trie_free(trie_t **p_trie)
{
	trie_free_subtrie(*p_trie, (*p_trie)->root);
	free(*p_trie);
}
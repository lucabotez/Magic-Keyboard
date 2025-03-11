// Copyright @lucabotez

#include "mk.h"
#include "trie.h"

// function that computes and returns the number of different letters between
// 2 words of the same length
int difference(char *word1, char *word2)
{
	int num = 0;
	for (int i = 0; i < strlen(word1); i++)
		if (word1[i] != word2[i])
			num++;

	return num;
}

// function that iterates through the trie, checking for each word if there are more
// than num different letters and is the same length with a given word, printing them
// in alphabetical order
void autocorrect(trie_node_t *node, char *new_word, int length, char *word,
				 int num, int *check)
{
	if (node->end_of_word) {
		new_word[length] = '\0';

		// verificam daca sunt respectate conditiile
		// checking if the conditions are respected
		if (length == strlen(word) && difference(word, new_word) <= num) {
			printf("%s\n", new_word);

			// marking that at least one word was printed
			*check = 1;
		}
	}

	// recursive trie iteration
	for (int i = 0; i < 26; i++)
		if (node->children[i]) {
			new_word[length] = i + 'a';

			autocorrect(node->children[i], new_word, length + 1,
						word, num, check);
		}
}

// function that finds and prints the first word that matches the prefix
void autocomplete_1(trie_node_t *node, char *word, int length)
{
	// forming the word
	while (node->end_of_word == 0)
		for (int i = 0; i < 26; i++)
			if (node->children[i]) {
				word[length] = i + 'a';
				length++;

				node = node->children[i];
				break;
			}

	word[length] = '\0';
	printf("%s\n", word);
}

// function that finds the shortest word that matches the prefix
void autocomplete_2(trie_node_t *node, char *prefix, char *shortest,
					int length, int *first)
{
	if (node->end_of_word) {
		prefix[length] = '\0';

		// length is not checked for the first word
		if (*first == 1) {
			*first = 0;
			strcpy(shortest, prefix);
		} else if (length < strlen(shortest)) {
			strcpy(shortest, prefix);
		}
	}

	// recursive trie iteration
	for (int i = 0; i < 26; i++)
		if (node->children[i]) {
			prefix[length] = i + 'a';
			autocomplete_2(node->children[i], prefix, shortest,
						   length + 1, first);
		}
}

// function that finds the most frequently used word that matches the prefix
void autocomplete_3(trie_node_t *node, char *prefix, char *freq,
					int *prev_freq, int length)
{
	if (node->end_of_word) {
		prefix[length] = '\0';

		// comparing the use frequency of the found word with the best found
		// frequency
		if ((*prev_freq) < node->frequency) {
			strcpy(freq, prefix);
			*prev_freq = node->frequency;
		}
	}

	// recursive trie iteration
	for (int i = 0; i < 26; i++)
		if (node->children[i]) {
			prefix[length] = i + 'a';
			autocomplete_3(node->children[i], prefix, freq,
						   prev_freq, length + 1);
		}
}

// function that handles the autocomplete request, calling the proper helper
// function (autocomplete1/2/3)
void autocomplete_prep(trie_t *trie, char *prefix, int num)
{
	trie_node_t *node = trie->root;

	// storing the last node that forms the prefix, using it as root in
	// autocomplete
	for (int i = 0; i < strlen(prefix); i++) {
		// prefix not found case
		if (!node->children[prefix[i] - 'a']) {
			printf("No words found\n");

			// 0 autocomplete case
			if (num == 0) {
				printf("No words found\n");
				printf("No words found\n");
			}
			return;
		}

		node = node->children[prefix[i] - 'a'];
	}

	// prefix copy, the original is lost after calling the helper functions
	char prefix_copy[WORD_SIZE];
	strcpy(prefix_copy, prefix);

	if (num == 1 || num == 0)
		autocomplete_1(node, prefix, strlen(prefix));

	if (num == 2 || num == 0) {
		// restoring the original value
		strcpy(prefix, prefix_copy);

		char shortest[WORD_SIZE]; // matched word
		int first = 1;      // if the word is the first one found

		autocomplete_2(node, prefix, shortest, strlen(prefix), &first);
		printf("%s\n", shortest);
	}

	if (num == 3 || num == 0) {
		// restoring the original value
		strcpy(prefix, prefix_copy);

		char freq[WORD_SIZE];    // matched word
		int prev_freq = 0; // stored word frequency

		autocomplete_3(node, prefix, freq, &prev_freq, strlen(prefix));
		printf("%s\n", freq);
	}
}

int main(void)
{
	trie_t *trie = trie_create(sizeof(char), 26);
	char command[WORD_SIZE]; // received command
	char word[WORD_SIZE];    // the word that is going to be added / autocorrected

	while (1) {
		fscanf(stdin, "%s", command);

		if (strcmp(command, "INSERT") == 0) {
			fscanf(stdin, "%s", word);
			trie_insert(trie, word);
		} else if (strcmp(command, "LOAD") == 0) {
			char filename[WORD_SIZE];
			fscanf(stdin, "%s", filename);

			FILE *in = fopen(filename, "rt");
			DIE(!in, "failed to open file");

			// receiving the file database
			do {
				fscanf(in, "%s", word);
				trie_insert(trie, word);
			} while (feof(in) == 0);

			trie_remove(trie, word);

			fclose(in);
		} else if (strcmp(command, "AUTOCORRECT") == 0) {
			int num, check = 0;
			fscanf(stdin, "%s%d", word, &num);

			char new[WORD_SIZE]; // autocorrect result
			autocorrect(trie->root, new, 0, word, num, &check);

			// no word found case
			if (check == 0)
				printf("No words found\n");
		} else if (strcmp(command, "AUTOCOMPLETE") == 0) {
			char prefix[WORD_SIZE];
			int num; // number of used autocomplete

			fscanf(stdin, "%s%d", prefix, &num);
			autocomplete_prep(trie, prefix, num);
		} else if (strcmp(command, "REMOVE") == 0) {
			fscanf(stdin, "%s", word);
			trie_remove(trie, word);
		} else if (strcmp(command, "EXIT") == 0) {
			trie_free(&trie);
			break;
		}
	}
	return 0;
}


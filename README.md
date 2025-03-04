Copyright @lucabotez

# Magic Keyboards

## Overview
This program implements a **Trie data structure** to support text-based operations such as word insertion, removal, autocomplete, and autocorrect. The trie is dynamically allocated and efficiently handles large amounts of word data.

## Features
- **Insert words** into the trie.
- **Load words** from a file.
- **Autocorrect** words with minor differences.
- **Autocomplete** words based on given prefixes.
- **Remove words** from the trie.
- **Efficient memory management**, with dynamic allocation and deallocation.

## Commands

### 1) INSERT
- **Usage:** `INSERT <word>`
- **Description:** Inserts a new word into the trie. If the word already exists, its frequency is incremented.

### 2) LOAD
- **Usage:** `LOAD <filename>`
- **Description:** Reads words from a file and inserts them into the trie.

### 3) AUTOCORRECT
- **Usage:** `AUTOCORRECT <word> <num>`
- **Description:** Suggests words from the trie that differ from `<word>` by at most `<num>` letters and have the same length.

### 4) AUTOCOMPLETE
- **Usage:** `AUTOCOMPLETE <prefix> <num>`
- **Description:** Suggests words based on the given prefix using different autocomplete strategies:
  - `1`: First word found in lexicographic order.
  - `2`: Shortest word with the given prefix.
  - `3`: Most frequently inserted word with the prefix.
  - `0`: Runs all three autocomplete strategies.

### 5) REMOVE
- **Usage:** `REMOVE <word>`
- **Description:** Removes a word from the trie. If the word is part of another longer word, only the specific entry is removed.

### 6) EXIT
- **Usage:** `EXIT`
- **Description:** Frees all dynamically allocated memory and terminates the program.

## Implementation Details
- **Trie Structure:**
  - Each node has an array of pointers to child nodes, supporting 26 lowercase English letters.
  - Each node tracks whether it marks the end of a word and stores its frequency.
- **Memory Management:**
  - Nodes are dynamically allocated using `malloc()`.
  - Proper cleanup functions ensure no memory leaks (`trie_free()` and `trie_remove()`).
- **Performance Considerations:**
  - Autocomplete and autocorrect operations are optimized for quick traversal.
  - Trie insertion and lookup have an average time complexity of **O(n)**, where `n` is the length of the word.

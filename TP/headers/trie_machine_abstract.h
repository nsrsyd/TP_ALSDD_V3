#ifndef TRIE_MACHINE_ABSTRACT_H
#define TRIE_MACHINE_ABSTRACT_H

#include <stdbool.h>

typedef struct {
    char word1[256], word2[256], word3[256];
    int count1, count2, count3;
} TopWords;

typedef struct Trie_node {
  struct Trie_node *children[26];
  bool is_end_of_word;
  int count;
} Trie_node;

typedef struct Trie {
  Trie_node *Root;
} Trie;

Trie *create_trie();
void prep_trie_node(Trie_node *node);
Trie_node *create_trie_node();
void insert(Trie *trie, char *s, int count);
bool search(Trie *trie, char *s);
bool has_children(Trie_node *node);
Trie_node *help_delete_word(Trie_node *root, char *s, int i);
void delete_word(Trie_node **root, char *s);
void help_free_trie(Trie_node *node);
void free_Trie(Trie *trie);
bool start_with(Trie *trie, char *s);
TopWords top_three_words(Trie *trie);
void help_print_words(Trie_node *root, char *word, int j);
void print_words(Trie *trie);

#endif // TRIE_MACHINE_ABSTRACT_H
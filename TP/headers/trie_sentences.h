#ifndef TRIE_SENTENCES_H
#define TRIE_SENTENCES_H

#include <stdbool.h>

typedef struct Trie_node_S {
  struct Trie_node_S *children[30];
  bool is_end_of_sentence;
  int count;
} Trie_node_S;

typedef struct Trie_S {
  Trie_node_S *Root;
} Trie_S;

Trie_node_S *create_trie_node_S();
Trie_S *create_trie_S();
int get_index_S(char c);
void insert_S(Trie_S *trie, char *s, int count);
bool search_S(Trie_S *trie, char *s);
bool has_children_S(Trie_node_S *node);
void delete_sentence_S(Trie_node_S **root, char *s);
void free_Trie_S(Trie_S *trie);
bool start_with_S(Trie_S *trie, char *s);
void print_sentences_S(Trie_S *trie);

#endif

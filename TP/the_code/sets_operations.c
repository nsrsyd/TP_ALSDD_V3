#include "../headers/sets_operations.h"
#include <stdlib.h>
#include <string.h>

int min(int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}

void copying_trie(Trie_node *node, char *word, int i, Trie_node *root) {
  if (node == NULL || root == NULL) {
    return;
  }
  if (node->is_end_of_word) {
    word[i] = '\0';
    Trie temp = {root};
    insert(&temp, word, node->count);
  }
  for (int j = 0; j < 26; j++) {
    if (node->children[j] != NULL) {
      word[i] = 'a' + j;
      copying_trie(node->children[j], word, i + 1, root);
    }
  }
}

Trie *union_a_b(Trie *t1, Trie *t2) {
  Trie *new_trie = create_trie();
  char word[1000];
  copying_trie(t1->Root, word, 0, new_trie->Root);
  copying_trie(t2->Root, word, 0, new_trie->Root);
  return new_trie;
}

void traverse_inter(Trie_node *node1, Trie_node *node2, char *word, int i,
                   Trie_node *root) {
  if (node1 == NULL || node2 == NULL || root == NULL) {
    return;
  }
  if (node1->is_end_of_word && node2->is_end_of_word) {
    word[i] = '\0';
    Trie temp = {root};
    insert(&temp, word, min(node1->count, node2->count));
  }
  for (int j = 0; j < 26; j++) {
    if (node1->children[j] != NULL && node2->children[j] != NULL) {
      word[i] = 'a' + j;
      traverse_inter(node1->children[j], node2->children[j], word, i + 1,
                         root);
    }
  }
}

Trie *intersection(Trie *t1, Trie *t2) {
  Trie *new_trie = create_trie();
  char word[1000];
  traverse_inter(t1->Root, t2->Root, word, 0, new_trie->Root);

  return new_trie;
}

void help_traverse_diff(Trie_node *node1, Trie_node *node2, Trie *trie, char *word,
                  int i) {
  if (node1 == NULL || trie == NULL || node2 == NULL) {
    return;
  }
  if (node1->is_end_of_word) {
    word[i] = '\0';
    Trie temp_search = {node2};
    if (!search(&temp_search, word)) {
      insert(trie, word, node1->count);
    }
  }
  for (int j = 0; j < 26; j++) {
    if (node1->children[j] != NULL) {
      word[i] = 'a' + j;
      help_traverse_diff(node1->children[j], node2, trie, word, i + 1);
    }
  }
}

Trie *difference(Trie *t1, Trie *t2) {
  Trie *new_trie = create_trie();
  if (t1 == NULL || t1->Root == NULL || t2 == NULL)
    return new_trie;
  char word[1000];
  help_traverse_diff(t1->Root, t2->Root, new_trie, word, 0);
  return new_trie;
}

Trie *copy_trie(Trie *trie) {
  Trie *new_trie = create_trie();
  if (trie == NULL || trie->Root == NULL)
    return new_trie;
  char word[1000];
  copying_trie(trie->Root, word, 0, new_trie->Root);
  return new_trie;
}

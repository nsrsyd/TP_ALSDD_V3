#include "../headers/sets_sentences.h"
#include <stdlib.h>
#include <string.h>

#define MAX_SENTENCES 100000

static int min_S(int a, int b) {
  return (a < b) ? a : b;
}

void copying_trie_S(Trie_node_S *node, char *sentence, int i,
                    Trie_node_S *trie) {
  if (node == NULL || trie == NULL)
    return;
  if (node->is_end_of_sentence) {
    sentence[i] = '\0';
    Trie_S temp = {trie};
    insert_S(&temp, sentence, node->count);
  }
  for (int j = 0; j < 30; j++) {
    if (node->children[j] != NULL) {
      if (j < 26)
        sentence[i] = 'a' + j;
      else if (j == 26)
        sentence[i] = ' ';
      else if (j == 27)
        sentence[i] = ',';
      else if (j == 28)
        sentence[i] = ';';
      else if (j == 29)
        sentence[i] = ':';
      copying_trie_S(node->children[j], sentence, i + 1, trie);
    }
  }
}

Trie_S* copy_trie_S(Trie_S* trie) {
  Trie_S *new_trie = create_trie_S();
  if (trie == NULL || trie->Root == NULL)
    return new_trie;
  char word[1000];
  copying_trie_S(trie->Root, word, 0, new_trie->Root);
  return new_trie;
}

Trie_S *union_a_b_S(Trie_S *t1, Trie_S *t2) {
  Trie_S *new_trie = create_trie_S();
  char sentence[1000];
  copying_trie_S(t1->Root, sentence, 0, new_trie->Root);
  copying_trie_S(t2->Root, sentence, 0, new_trie->Root);
  return new_trie;
}

void travers_inter_S(Trie_node_S *node1, Trie_node_S *node2, char *sentence, int i,
                     Trie_node_S *res_node) {
  if (node1 == NULL || node2 == NULL || res_node == NULL)
    return;
  if (node1->is_end_of_sentence && node2->is_end_of_sentence) {
    sentence[i] = '\0';
    Trie_S temp = {res_node};
    insert_S(&temp, sentence, min_S(node1->count, node2->count));
  }
  for (int j = 0; j < 30; j++) {
    if (node1->children[j] != NULL && node2->children[j] != NULL) {
      if (j < 26)
        sentence[i] = 'a' + j;
      else if (j == 26)
        sentence[i] = ' ';
      else if (j == 27)
        sentence[i] = ',';
      else if (j == 28)
        sentence[i] = ';';
      else if (j == 29)
        sentence[i] = ':';
      travers_inter_S(node1->children[j], node2->children[j], sentence,
                           i + 1, res_node);
    }
  }
}

Trie_S *intersection_S(Trie_S *t1, Trie_S *t2) {
  Trie_S *new_trie = create_trie_S();
  char sentence[1000];
  travers_inter_S(t1->Root, t2->Root, sentence, 0, new_trie->Root);
  return new_trie;
}

void travers_diff_S(Trie_node_S *node1, Trie_node_S *node2, Trie_S *trie,
                    char *sentence, int i) {
  if (node1 == NULL || trie == NULL || node2 == NULL) {
    return;
  }
  if (node1->is_end_of_sentence) {
    sentence[i] = '\0';
    Trie_S temp_search = {node2};
    if (!search_S(&temp_search, sentence)) {
      insert_S(trie, sentence, node1->count);
    }
  }
  for (int j = 0; j < 30; j++) {
    if (node1->children[j] != NULL) {
      if (j < 26)
        sentence[i] = 'a' + j;
      else if (j == 26)
        sentence[i] = ' ';
      else if (j == 27)
        sentence[i] = ',';
      else if (j == 28)
        sentence[i] = ';';
      else if (j == 29)
        sentence[i] = ':';
      travers_diff_S(node1->children[j], node2, trie, sentence, i + 1);
    }
  }
}

Trie_S *difference_S(Trie_S *t1, Trie_S *t2) {
  if (t1 == NULL || t1->Root == NULL)
    return create_trie_S();
  if (t2 == NULL || t2->Root == NULL)
    return copy_trie_S(t1);
  Trie_S *new_trie = create_trie_S();
  char sentence[1000];
  travers_diff_S(t1->Root, t2->Root, new_trie, sentence, 0);

  return new_trie;
}

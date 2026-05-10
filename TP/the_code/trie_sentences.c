#include "../headers/trie_sentences.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prep_trie_node_S(Trie_node_S *node) {
  if (!node)
    return;
  node->is_end_of_sentence = false;
  node->count = 0;
  for (int i = 0; i < 30; i++) {
    node->children[i] = NULL;
  }
}

Trie_node_S *create_trie_node_S() {
  Trie_node_S *node = malloc(sizeof(Trie_node_S));
  if (node)
    prep_trie_node_S(node);
  return node;
}

Trie_S *create_trie_S() {
  Trie_S *trie = malloc(sizeof(Trie_S));
  if (trie) {
    trie->Root = create_trie_node_S();
  }
  return trie;
}

int get_index_S(char c) {
  if (c >= 'a' && c <= 'z')
    return c - 'a';
  if (c == ' ')
    return 26;
  if (c == ',')
    return 27;
  if (c == ';')
    return 28;
  if (c == ':')
    return 29;
  return -1;
}

void insert_S(Trie_S *trie, char *s, int count) {
  if (!trie || !trie->Root || !s || !(*s))
    return;
  Trie_node_S *curr = trie->Root;
  int size = strlen(s);
  for (int i = 0; i < size; i++) {
    int index = get_index_S(s[i]);
    if (index == -1)
      continue;
    if (curr->children[index] == NULL) {
      curr->children[index] = create_trie_node_S();
    }
    curr = curr->children[index];
  }
  curr->is_end_of_sentence = true;
  curr->count += count;
}

bool search_S(Trie_S *trie, char *s) {
  if (!trie || !trie->Root || !s || !(*s))
    return false;
  Trie_node_S *curr = trie->Root;
  int size = strlen(s);
  for (int i = 0; i < size; i++) {
    int index = get_index_S(s[i]);
    if (index == -1)
      continue;
    if (curr->children[index] == NULL)
      return false;
    curr = curr->children[index];
  }
  return curr->is_end_of_sentence;
}

bool has_children_S(Trie_node_S *node) {
  if (!node)
    return false;
  for (int i = 0; i < 30; i++) {
    if (node->children[i] != NULL)
      return true;
  }
  return false;
}

Trie_node_S *help_delete_sentence_S(Trie_node_S *root, char *s, int i) {
  if (!root)
    return NULL;

  if (s[i] == '\0') {
    if (root->is_end_of_sentence) {
      root->is_end_of_sentence = false;
    }
    if (!has_children_S(root)) {
      free(root);
      return NULL;
    }
    return root;
  }

  int index = get_index_S(s[i]);
  if (index != -1) {
    root->children[index] =
        help_delete_sentence_S(root->children[index], s, i + 1);
  }

  if (!has_children_S(root) && !root->is_end_of_sentence) {
    free(root);
    return NULL;
  }
  return root;
}

void delete_sentence_S(Trie_node_S **root, char *s) {
  if (!root || !*root || !s)
    return;
  *root = help_delete_sentence_S(*root, s, 0);
}

void help_free_trie_S(Trie_node_S *node) {
  if (!node)
    return;
  for (int i = 0; i < 30; i++) {
    if (node->children[i])
      help_free_trie_S(node->children[i]);
  }
  free(node);
}

void free_Trie_S(Trie_S *trie) {
  if (!trie)
    return;
  help_free_trie_S(trie->Root);
  free(trie);
}

bool start_with_S(Trie_S *trie, char *s) {
  if (!trie || !trie->Root || !s)
    return false;
  Trie_node_S *curr = trie->Root;
  for (int i = 0; s[i] != '\0'; i++) {
    int index = get_index_S(s[i]);
    if (index == -1)
      return false;
    if (curr->children[index] == NULL)
      return false;
    curr = curr->children[index];
  }
  return true;
}

void help_print_sentence_S(Trie_node_S *root, char *sentence, int j) {
  if (!root)
    return;
  if (root->is_end_of_sentence) {
    sentence[j] = '\0';
    printf(" %s ", sentence);
  }
  for (int i = 0; i < 30; i++) {
    if (root->children[i] != NULL) {
      if (i < 26)
        sentence[j] = 'a' + i;
      else if (i == 26)
        sentence[j] = ' ';
      else if (i == 27)
        sentence[j] = ',';
      else if (i == 28)
        sentence[j] = ';';
      else if (i == 29)
        sentence[j] = ':';
      help_print_sentence_S(root->children[i], sentence, j + 1);
    }
  }
}

void print_sentences_S(Trie_S *trie) {
  if (!trie || !trie->Root)
    return;
  char sentence[1000];
  printf("{ ");
  help_print_sentence_S(trie->Root, sentence, 0);
  printf(" }");
}

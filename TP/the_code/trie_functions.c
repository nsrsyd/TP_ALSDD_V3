#include "../headers/trie_machine_abstract.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Trie *create_trie() {
  Trie *trie = malloc(sizeof(Trie));
  if (trie) {
    trie->Root = create_trie_node();
  }
  return trie;
}

void prep_trie_node(Trie_node *node) {
  if (node == NULL)
    return;
  node->is_end_of_word = false;
  node->count = 0;
  for (int i = 0; i < 26; i++) {
    node->children[i] = NULL;
  }
}

Trie_node *create_trie_node() {
  Trie_node *node = malloc(sizeof(Trie_node));
  prep_trie_node(node);
  return node;
}

void insert(Trie *trie, char *s, int count) {
  if (trie == NULL || trie->Root == NULL || s == NULL || *s == '\0') {
    return;
  }
  Trie_node *curr = trie->Root;
  int size = strlen(s);
  int index;
  for (int i = 0; i < size; i++) {
    index = s[i] - 'a';
    if (index < 0 || index >= 26)
      continue; // Safety check
    if (curr->children[index] == NULL) {
      curr->children[index] = create_trie_node();
    }
    curr = curr->children[index];
  }
  curr->is_end_of_word = true;
  curr->count += count;
}

bool search(Trie *trie, char *s) {
  if (trie == NULL || trie->Root == NULL || s == NULL || *s == '\0') {
    return false;
  }
  Trie_node *curr = trie->Root;
  int size = strlen(s);
  int index;
  for (int i = 0; i < size; i++) {
    index = s[i] - 'a';
    if (index < 0 || index >= 26)
      return false;
    if (curr->children[index] == NULL) {
      return false;
    }
    curr = curr->children[index];
  }
  return curr->is_end_of_word;
}

bool has_children(Trie_node *node) {
  if (node == NULL) {
    return false;
  }
  for (int i = 0; i < 26; i++) {
    if (node->children[i] != NULL) {
      return true;
    }
  }
  return false;
}

Trie_node *help_delete_word(Trie_node *root, char *s, int i) {
  if (!root)
    return NULL;

  if (s[i] == '\0') {
    if (root->is_end_of_word) {
      root->is_end_of_word = false;
    }

    if (!has_children(root)) {
      free(root);
      return NULL;
    }
    return root;
  }

  int index = s[i] - 'a';
  if (index >= 0 && index < 26) {
    root->children[index] = help_delete_word(root->children[index], s, i + 1);
  }

  if (!has_children(root) && !root->is_end_of_word) {
    free(root);
    return NULL;
  }

  return root;
}

void delete_word(Trie_node **root, char *s) {
  if (root == NULL || *root == NULL || s == NULL)
    return;
  *root = help_delete_word(*root, s, 0);
}

void help_free_trie(Trie_node *node) {
  if (node == NULL) {
    return;
  }
  for (int i = 0; i < 26; i++) {
    if (node->children[i] != NULL) {
      help_free_trie(node->children[i]);
    }
  }
  free(node);
}

void free_Trie(Trie *trie) {
  if (trie == NULL) {
    return;
  }
  help_free_trie(trie->Root);
  free(trie);
}

bool start_with(Trie *trie, char *s) {
  if (trie == NULL || trie->Root == NULL || s == NULL) {
    return false;
  }
  Trie_node *curr = trie->Root;
  int index;
  for (int i = 0; s[i] != '\0'; i++) {
    index = s[i] - 'a';
    if (index < 0 || index >= 26)
      return false;
    if (curr->children[index] == NULL) {
      return false;
    }
    curr = curr->children[index];
  }
  return true;
}

void help_top_three(Trie_node *node, char *word, int i, TopWords *tw) {
  if (node == NULL) return;
  if (node->is_end_of_word) {
    word[i] = '\0';
    int c = node->count;
    if (c > tw->count1) {
      strcpy(tw->word3, tw->word2); 
      tw->count3 = tw->count2;
      strcpy(tw->word2, tw->word1); 
      tw->count2 = tw->count1;
      strcpy(tw->word1, word);      
      tw->count1 = c;
    } else if (c > tw->count2) {
      strcpy(tw->word3, tw->word2); 
      tw->count3 = tw->count2;
      strcpy(tw->word2, word);      
      tw->count2 = c;
    } else if (c > tw->count3) {
      strcpy(tw->word3, word);      
      tw->count3 = c;
    }
  }
  for (int j = 0; j < 26; j++) {
    if (node->children[j] != NULL) {
      word[i] = 'a' + j;
      help_top_three(node->children[j], word, i + 1, tw);
    }
  }
}

TopWords top_three_words(Trie *trie) {
  TopWords tw;
  memset(&tw, 0, sizeof(TopWords));
  if (trie == NULL || trie->Root == NULL) return tw;
  char current_word[256] = "";
  help_top_three(trie->Root, current_word, 0, &tw);
  return tw;
}

void help_print_words(Trie_node *root, char *word, int j) {
  if (root == NULL) {
    return;
  }
  if (root->is_end_of_word) {
    word[j] = '\0';
    printf(" %s ", word);
  }
  for (int i = 0; i < 26; i++) {
    if (root->children[i] != NULL) {
      word[j] = 'a' + i;
      help_print_words(root->children[i], word, j + 1);
    }
  }
}

void print_words(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return;
  }
  char word[100];
  printf("{ ");
  help_print_words(trie->Root, word, 0);
  printf(" }");
}

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
    return; // if the words is empty or the trie or its root does not even exist , we quit.
  }
  Trie_node *curr = trie->Root;
  int size = strlen(s); 
  int index;
  for (int i = 0; i < size; i++) {
    index = s[i] - 'a'; // to get the index of the node that we need to go to .
    if (index < 0 || index >= 26)
      continue; // Safety check
    if (curr->children[index] == NULL) {
      curr->children[index] = create_trie_node(); // if the node in that index does not exit (which mean the path does not exist , we creat one) we creat a node.
    }
    curr = curr->children[index]; // we move to the node of the next letter.
  }
  curr->is_end_of_word = true; // after rueaching the end of the word we insert it by putting the field is_end_of_word True (this mean that the sum of all letter until this node (the path) is a valid word)
  curr->count += count; // we increament the number of the appearances of the word by one.
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
      return false; // we quit if we encounter a non alphabitical char.
    if (curr->children[index] == NULL) {
      return false; // if the node does not exist , this mean that the path to that word does not exist which mean that the word was not been inserted to the trie .
    }
    curr = curr->children[index]; // we move to the node of the next char in the word.
  }
  return curr->is_end_of_word; // if is_end_of_word is true , this mean that the word exists in the trie , if not , this mean the opposite.
}

bool has_children(Trie_node *node) {
  if (node == NULL) {
    return false;
  }
  for (int i = 0; i < 26; i++) {
    if (node->children[i] != NULL) {
      return true; // if we find just one non NULL node , this mean that a child exists , so we return True directly
    }
  }
  return false; // if we reach this line , this mean that all the pointers array is NULLs , so no child exists.
}

Trie_node *help_delete_word(Trie_node *root, char *s, int i) {
  if (!root)
    return NULL;

  if (s[i] == '\0')  { // this is the base case
    if (root->is_end_of_word) {
      root->is_end_of_word = false; // if we reach the end of the word we set the boolean to false to indicat that the word does not exist in the trie anymore.
    }

    if (!has_children(root)) {
      free(root); // if the node does not have any child , we free it directly beacuse this will not affect anything (any path to a word) and we dont need the node anymore.
      return NULL; // after freeing the node , we return NULL to assign it to its index in the pointers array of its parent .
    }
    return root; // if the node has children , we do not free it and return its address directly , so its parent is still having that node as a child .
  } // all this will work just in the node of the last char.

  int index = s[i] - 'a'; // this for getting the index of the next node that we are goin to.
  if (index >= 0 && index < 26) {
    root->children[index] = help_delete_word(root->children[index], s, i + 1); // recusive call of the function for the traversing .
  }

  if (!has_children(root) && !root->is_end_of_word) {
    free(root); // if the node does not have any child and it is not the end of a word , we free it because we don't need it
    return NULL; // the same as before.
  }

  return root;
}

void delete_word(Trie_node **root, char *s) {
  if (root == NULL || *root == NULL || s == NULL) 
    return;
  *root = help_delete_word(*root, s, 0); // this will handle everything.
}

void help_free_trie(Trie_node *node) {
  if (node == NULL) {
    return;
  }
  for (int i = 0; i < 26; i++) {
    if (node->children[i] != NULL) { // if node->children[i] == NULL , we dont need to call the function , because there is nothing to free.
      help_free_trie(node->children[i]); // we move to the existing nodes to free them.
    }
  }
  free(node); // we start freeing from the bottom of the trie like postorder.
}

void free_Trie(Trie *trie) {
  if (trie == NULL) {
    return;
  }
  help_free_trie(trie->Root); // this will handle everything.
  free(trie); // finally we free the complete struture of
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

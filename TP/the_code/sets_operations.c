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
    Trie temp = {root}; // this is for just creating a trie where its root is the root that we pass to the function , just that we can pass it to the insert function
    insert(&temp, word, node->count); // after constructing a complete word , we insert it in our new trie.
  }
  for (int j = 0; j < 26; j++) { // we loop over all the childrens of the node and call the recursive funcion on them.
    if (node->children[j] != NULL) { // this mean that there exist a path to that char (the ascii code of that char is the ascii code of "a" plus the index of its node in the pointers array )
      word[i] = 'a' + j;
      copying_trie(node->children[j], word, i + 1, root);
    }
  }
}

Trie *union_a_b(Trie *t1, Trie *t2) {
  Trie *new_trie = create_trie();
  char word[1000];
  copying_trie(t1->Root, word, 0, new_trie->Root); // we make a copy of the first trie in the new trie 
  copying_trie(t2->Root, word, 0, new_trie->Root); // we then insert all the words of the second trie into our new trie (the copying_trie allow us to do that) 
  return new_trie; // now , new_trie contains all the words of t1 and t2 with the number of its appearances.
}

void traverse_inter(Trie_node *node1, Trie_node *node2, char *word, int i,
                   Trie_node *root) {
  if (node1 == NULL || node2 == NULL || root == NULL) {
    return;
  }
  if (node1->is_end_of_word && node2->is_end_of_word) { // if we find a complete word that exist in both tries
    word[i] = '\0';
    Trie temp = {root}; // the same as before
    insert(&temp, word, min(node1->count, node2->count)); // we insert the word in our new_trie
  }
  for (int j = 0; j < 26; j++) {
    if (node1->children[j] != NULL && node2->children[j] != NULL) { // we just visit the paths that exist in both tries
      word[i] = 'a' + j; 
      traverse_inter(node1->children[j], node2->children[j], word, i + 1,
                         root);
    }
  }
}

Trie *intersection(Trie *t1, Trie *t2) {
  Trie *new_trie = create_trie();
  char word[1000]; // we hope that a word does not exceed a 1000 char :)
  traverse_inter(t1->Root, t2->Root, word, 0, new_trie->Root); // this will handle evrything

  return new_trie;
}

void help_traverse_diff(Trie_node *node1, Trie_node *node2, Trie *trie, char *word,
                  int i) {
  if (node1 == NULL || trie == NULL || node2 == NULL) {
    return;
  }
  if (node1->is_end_of_word) { // after finding a complete word
    word[i] = '\0';
    Trie temp_search = {node2};
    if (!search(&temp_search, word)) { // we check if it does not exist in the other trie 
      insert(trie, word, node1->count); // if yes we insert it in our new_trie 
    }
  }
  for (int j = 0; j < 26; j++) {
    if (node1->children[j] != NULL) {
      word[i] = 'a' + j; // the same as before
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

Trie *copy_trie(Trie *trie) { // honestly , i forgot why i made this function 
  Trie *new_trie = create_trie();
  if (trie == NULL || trie->Root == NULL)
    return new_trie;
  char word[1000];
  copying_trie(trie->Root, word, 0, new_trie->Root); 
  return new_trie;
}

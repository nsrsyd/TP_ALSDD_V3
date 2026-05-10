#ifndef SETS_OPERATIONS_H
#define SETS_OPERATIONS_H

#include "trie_machine_abstract.h"

void help_traverse(Trie_node *node, char *word, int i, char **res, int *k);
Trie *union_a_b(Trie *t1, Trie *t2);
Trie *intersection(Trie *t1, Trie *t2);
void help_traverse_diff(Trie_node *node1, Trie_node *node2, Trie *trie, char *word, int i);
Trie *difference(Trie *t1, Trie *t2);
Trie *copy_trie(Trie *trie);

#endif 

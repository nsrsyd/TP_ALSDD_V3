#ifndef FILTERING_EXTRACTION_H
#define FILTERING_EXTRACTION_H

#include "trie_machine_abstract.h"

void help_traverse_num(Trie_node *node, int *cpt);
int num_of_words(Trie *A);
double calculateJaccard(Trie *a, Trie *b);
void remove_stopwords(Trie *trie);
void remove_conjunctions(Trie *trie);
void remove_pronouns(Trie *trie);
int check_similarity(Trie *a, Trie *b);
TopWords topic(Trie *trie);
Trie_node *go_to(Trie *trie, char *root);
void help_travers_2(Trie_node *node, char *word, int i, char **res, int *k);
char **words_start_with(Trie *trie, char *s);
void helper_count(Trie_node *node, int *cpt);
int countPrefixMatches(Trie *trie, char *prefix);
void get_min_max_word(Trie_node *node, char *min_word, char *max_word,
                      char *word, int i, int *max, int *min, int *num_words,
                      int *num_unique_words);
void words_stats(Trie *trie);
void statistics(Trie *trie);

#endif // FILTERING_EXTRACTION_H
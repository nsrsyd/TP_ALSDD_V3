#ifndef FILTERING_SENTENCES_H
#define FILTERING_SENTENCES_H

#include "sets_sentences.h"

int num_of_sentences_S(Trie_S* A);
double calculateJaccard_S(Trie_S* a, Trie_S* b);
char** sentences_start_with_S(Trie_S* trie, char* s);
int countPrefixMatches_S(Trie_S* trie, char* prefix);

#endif

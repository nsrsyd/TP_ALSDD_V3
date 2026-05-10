#ifndef SETS_SENTENCES_H
#define SETS_SENTENCES_H

#include "trie_sentences.h"

Trie_S* union_a_b_S(Trie_S* t1, Trie_S* t2);
Trie_S* intersection_S(Trie_S* t1, Trie_S* t2);
Trie_S* difference_S(Trie_S* t1, Trie_S* t2);
Trie_S* copy_trie_S(Trie_S* trie);

#endif

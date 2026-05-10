#include "../headers/filtering_sentences.h"
#include <stdlib.h>
#include <string.h>

void help_travers_num_S(Trie_node_S* node, int* cpt) {
    if (node == NULL) return;
    if (node->is_end_of_sentence) {
        (*cpt) += node->count;
    }
    for (int j = 0; j < 30; j++) {
        if (node->children[j] != NULL) {
            help_travers_num_S(node->children[j], cpt);
        }
    }
}

int num_of_sentences_S(Trie_S* A) {
    if (A == NULL || A->Root == NULL) return 0;
    int cpt = 0;
    help_travers_num_S(A->Root, &cpt);
    return cpt;
}

double calculateJaccard_S(Trie_S* a, Trie_S* b) {
    Trie_S* unionn = union_a_b_S(a, b);
    Trie_S* inter = intersection_S(a, b);
    int n1 = num_of_sentences_S(inter);
    int n2 = num_of_sentences_S(unionn);
    double result = 0.0;
    if (n2 != 0) {
        result = (double)n1 / (double)n2;
    }
    free_Trie_S(unionn);
    free_Trie_S(inter);
    return result;
}

Trie_node_S* go_to_S(Trie_S* trie, char* root) {
    if (!trie || !trie->Root || !root) return NULL;
    Trie_node_S* curr = trie->Root;
    for (int i = 0; root[i] != '\0'; i++) {
        int index = get_index_S(root[i]);
        if (index == -1) return NULL;
        if (curr->children[index] == NULL) return NULL;
        curr = curr->children[index];
    }
    return curr;
}

void help_travers_2_S(Trie_node_S* node, char* sentence, int i, char** res, int* k) {
    if (node == NULL || *k >= 1000) return;
    if (node->is_end_of_sentence) {
        sentence[i] = '\0';
        res[(*k)++] = strdup(sentence);
        if (*k >= 1000) return;
    }
    for (int j = 0; j < 30; j++) {
        if (node->children[j] != NULL) {
            if (j < 26) sentence[i] = 'a' + j;
            else if (j == 26) sentence[i] = ' ';
            else if (j == 27) sentence[i] = ',';
            else if (j == 28) sentence[i] = ';';
            else if (j == 29) sentence[i] = ':';
            help_travers_2_S(node->children[j], sentence, i + 1, res, k);
        }
    }
}

char** sentences_start_with_S(Trie_S* trie, char* s) {
    if (!trie || !trie->Root || !s) return NULL;
    Trie_node_S* node = go_to_S(trie, s);
    if (!node) return NULL;
    char** words = malloc(sizeof(char*) * 1001); 
    int k = 0;
    char sentence[1000];
    strcpy(sentence, s);
    int size = strlen(s);
    help_travers_2_S(node, sentence, size, words, &k);
    words[k] = NULL;
    return words;
}

void helper_count_S(Trie_node_S* node, int* cpt) {
    if (node == NULL) return;
    if (node->is_end_of_sentence) {
        (*cpt) += node->count;
    }
    for (int i = 0; i < 30; i++) {
        if (node->children[i] != NULL) {
            helper_count_S(node->children[i], cpt);
        }
    }
}

int countPrefixMatches_S(Trie_S* trie, char* prefix) {
    if (!trie || !trie->Root || !prefix) return 0;
    Trie_node_S* node = go_to_S(trie, prefix);
    if (!node) return 0;
    int cpt = 0;
    helper_count_S(node, &cpt);
    return cpt;
}

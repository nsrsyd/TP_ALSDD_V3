#include "../headers/filtering_extraction.h"
#include "../headers/sets_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void help_traverse_num(Trie_node *node, int *cpt) {
  if (node == NULL || cpt == NULL) {
    return;
  }
  if (node->is_end_of_word) {
    (*cpt) += node->count; // after we find a word , we add the number of its appearances to the total counter
  }
  for (int j = 0; j < 26; j++) { // traversing as usual , all trie functions have this bloque of code in common , so i dont need to explain it again.
    if (node->children[j] != NULL) {
      help_traverse_num(node->children[j], cpt);
    }
  }
}

int num_of_words(Trie *A) {
  if (A == NULL || A->Root == NULL) {
    return 0;
  }
  int cpt = 0;
  help_traverse_num(A->Root, &cpt); // it is obvious i think.
  return cpt;
}

double calculateJaccard(Trie *a, Trie *b) {
  Trie *unionn = union_a_b(a, b);
  Trie *inter = intersection(a, b);
  int n1 = num_of_words(inter);
  int n2 = num_of_words(unionn);

  double result = 0.0;
  if (n2 != 0) {
    result = (double)n1 / (double)n2; // we use double for better accurucy and percision.
  }

  free_Trie(unionn); // to prevent a memory leak.
  free_Trie(inter); // to prevent a memory leak.
  return result;
}

void remove_stopwords(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return;
  }
  const char *stopwords[] = {
      "the",  "is",   "at",    "which",  "a",      "an",
      "and",  "or",   "but",   "in",     "on",     "to",
      "for",  "with", "about", "as",     "by",     "of",
      "from", "that", "this",  "not",    "are",    "was",
      "were", "been", "being", "have",   "has",    "had",
      "do",   "does", "did",   "will",   "would",  "shall",
      "should", "may", "might", "must",  "can",    "could",
      "no",   "nor",  "so",    "up",     "out",    "if",
      "then", "than", "too",   "very",   "just",   "into",
      "over", "after", "before", "between", "under", "such",
      "each", "every", "all",  "any",    "both",   "few",
      "more", "most", "other", "some",   "only",   "also",
      "how",  "when", "where", "why",    "what",   "there" 
  };
  int num_stopwords = sizeof(stopwords) / sizeof(stopwords[0]);
  for (int i = 0; i < num_stopwords; i++) {
    delete_word(&(trie->Root), (char *)stopwords[i]); // we delete all this words if we need to check something related the meaning , this words will anoy us.
  }
}

void remove_conjunctions(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return;
  }
  const char *conjunctions[] = {
      "however",     "therefore",  "moreover",  "thus",    "hence",
      "nevertheless", "furthermore", "although", "because", "since",
      "unless",      "while",      "also",      "yet",     "nor",
      "either",      "neither",    "whether",   "meanwhile", "instead",
      "otherwise",   "likewise",   "accordingly", "consequently",
      "still",       "once"
  };
  int num_conjunctions = sizeof(conjunctions) / sizeof(conjunctions[0]);
  for (int i = 0; i < num_conjunctions; i++) {
    delete_word(&(trie->Root), (char *)conjunctions[i]);
  }
} // the same as before

void remove_pronouns(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return;
  }
  const char *pronouns[] = {
      "he",    "she",    "they",   "it",       "i",      "we",
      "you",   "me",     "him",    "her",      "us",     "them",
      "my",    "your",   "his",    "their",    "its",    "our",
      "mine",  "yours",  "hers",   "theirs",   "ours",
      "who",   "whom",   "whose",  "what",     "which",
      "myself", "yourself", "himself", "herself", "itself",
      "ourselves", "themselves", "yourselves",
      "anybody",  "anyone",   "anything",
      "everybody", "everyone", "everything",
      "nobody",  "nothing",  "somebody",  "someone", "something",
      "these",   "those" 
  };
  int num_pronouns = sizeof(pronouns) / sizeof(pronouns[0]);
  for (int i = 0; i < num_pronouns; i++) {
    delete_word(&(trie->Root), (char *)pronouns[i]);
  }
} // the same as before.

int check_similarity(Trie *a, Trie *b) {
  if (a == NULL || a->Root == NULL || b == NULL || b->Root == NULL) {
    return 0;
  }
  remove_conjunctions(a);
  remove_pronouns(a);
  remove_stopwords(a);
  remove_conjunctions(b);
  remove_pronouns(b);
  remove_stopwords(b);
  return (int)(calculateJaccard(a, b) * 100); // to check the simmilarity in terms of the meaning (just words that has a meaning)
}

TopWords topic(Trie *trie) {
  TopWords empty;
  memset(&empty, 0, sizeof(TopWords));
  if (trie == NULL || trie->Root == NULL) {
    return empty;
  }
  remove_conjunctions(trie);
  remove_pronouns(trie);
  remove_stopwords(trie);
  return top_three_words(trie); // to get to most three used words (words that has a meaning)
}

Trie_node *go_to(Trie *trie, char *root) {
  if (trie == NULL || trie->Root == NULL || root == NULL) {
    return NULL;
  }
  Trie_node *curr = trie->Root;
  int index;
  int size = strlen(root);
  for (int i = 0; i < size; i++) {
    index = root[i] - 'a';
    if (index < 0 || index >= 26)
      return NULL;
    if (curr->children[index] == NULL) {
      return NULL;
    } else {
      curr = curr->children[index];
    }
  }
  return curr; // this is just a normal traversing as before .
}

void help_travers_2(Trie_node *node, char *word, int i, char **res, int *k) {
  if (node == NULL || res == NULL || k == NULL || *k >= 1000) {
    return;
  }
  if (node->is_end_of_word) {
    word[i] = '\0';
    res[(*k)++] = strdup(word); // we add the word to the array of words. then we increment the index k.
    if (*k >= 1000) 
      return; 
  }
  for (int j = 0; j < 26; j++) {
    if (node->children[j] != NULL) {
      word[i] = 'a' + j;
      help_travers_2(node->children[j], word, i + 1, res, k); // same as always.
    }
  }
}

char **words_start_with(Trie *trie, char *s) {
  if (trie == NULL || trie->Root == NULL || s == NULL) {
    return NULL;
  }
  Trie_node *node = go_to(trie, s); // this for moving to the node that all the nodes below it starts with that prefix/
  if (node == NULL) {
    return NULL;
  }
  char **words =
      malloc(sizeof(char *) * 1001); 
  int k = 0;
  char word[256];
  strcpy(word, s);
  int size = strlen(s);
  help_travers_2(node, word, size, words, &k); // we collect all the words belowe that node , all these words are gurented to start with that prefix
  words[k] = NULL;
  return words;
}

void helper_count(Trie_node *node, int *cpt) {
  if (node == NULL || cpt == NULL) {
    return;
  }
  if (node->is_end_of_word) {
    (*cpt) += node->count;
  }
  for (int i = 0; i < 26; i++) {
    if (node->children[i] != NULL) {
      helper_count(node->children[i], cpt);
    }
  }
}

int countPrefixMatches(Trie *trie, char *prefix) {
  if (trie == NULL || trie->Root == NULL || prefix == NULL) {
    return 0;
  }
  Trie_node *node = go_to(trie, prefix);
  if (node == NULL) {
    return 0;
  }
  int cpt = 0;
  helper_count(node, &cpt); // same as befor , but instead of collecting the words, we just count how many of them.
  return cpt;
}


void get_min_max_word(Trie_node *node, char *min_word, char *max_word,
                      char *word, int i, int *max, int *min, int *num_words,
                      int *num_unique_words) {
  if (node == NULL || max == NULL || min == NULL) {
    return;
  }
  if (node->is_end_of_word) { // if we find a complete word
    (*num_unique_words)++; // we increament the number the unique words/
    (*num_words) += node->count;// we increament the number of the total used words.
    if ((node->count) > (*max)) { // if it has a count bigger than the max , then it is the new max.
      word[i] = '\0';
      strcpy(max_word, word);
      (*max) = node->count;
    }
    if ((node->count) < (*min)) {// if it has a count less than the min , then it is the new min.
      if (word[i] != '\0') {
        word[i] = '\0'; // if there word was not a max we need to add this to ensure the end of a string for the complire to understand that it is aa end of a string.
      }
      strcpy(min_word, word); // for copying the word/
      (*min) = node->count;
    }
  }
  for (int j = 0; j < 26; j++) {
    if (node->children[j] != NULL) {
      word[i] = 'a' + j;
      get_min_max_word(node->children[j], min_word, max_word, word, i + 1, max,
                       min, num_words, num_unique_words); // as before.
    }
  }
}

void help_words_stats(Trie_node *root, char *word, int j, int num) {
  if (root == NULL) {
    return;
  }
  if (root->is_end_of_word) {
    word[j] = '\0';
    printf(" %s  : %d   %.2f%% \n", word, root->count,
           ((double)root->count / num) * 100);
  }
  for (int i = 0; i < 26; i++) {
    if (root->children[i] != NULL) {
      word[j] = 'a' + i;
      help_words_stats(root->children[i], word, j + 1, num);
    }
  }
}

void words_stats(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return;
  }
  char word[100];
  int total_words = num_of_words(trie);
  printf("the word    count   percentage  \n ");
  help_words_stats(trie->Root, word, 0, total_words);
  printf("\n =========================\n");
}

void statistics(Trie *trie) {
  if (trie == NULL || trie->Root == NULL) {
    return;
  }
  int min = INT_MAX;
  int max = 0;
  int num_of_words = 0;
  int num_of_unique_words = 0;
  char max_word[256];
  char min_word[256];
  char word[256];
  Trie_node *node = trie->Root;
  get_min_max_word(node, min_word, max_word, word, 0, &max, &min, &num_of_words,
                   &num_of_unique_words);

  printf("===================\n");
  printf("num of total words is : %d\n", num_of_words);
  printf("num of unique words is : %d\n", num_of_unique_words);
  printf("the most used word : %s\n", max_word);
  printf("num least used word : %s \n", min_word);
}

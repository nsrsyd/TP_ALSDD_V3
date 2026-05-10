#include "../headers/structure_sentences.h"
#include <stdlib.h>

// paragraph-level functions
para_S *create_para_S() {
  para_S *node = malloc(sizeof(para_S));
  if (node) {
    node->trie = create_trie_S();
    node->next = node->prev = NULL;
  }
  return node;
}
void para_ass_adr_trie_S(para_S *p, Trie_S *t) {
  if (p == NULL) return;
  p->trie = t;
  return;
}

void para_ass_adr_next_S(para_S *p, para_S *q) {
  if (p == NULL) return;
  p->next = q;
  return;
}

void para_ass_adr_prev_S(para_S *p, para_S *q) {
  if (p == NULL) return;
  p->prev = q;
  return;
}

Trie_S *para_get_trie_S(para_S *p) {
  if (p == NULL) return NULL;
  return p->trie;
}

para_S *para_get_next_S(para_S *p) {
  if (p == NULL) return NULL;
  return p->next;
}

para_S *para_get_prev_S(para_S *p) {
  if (p == NULL) return NULL;
  return p->prev;
}

// file-level fucntions 
file_S *create_file_S() {
  file_S *f = malloc(sizeof(file_S));
  if (f) {
    f->head = f->tail = NULL;
  }
  return f;
}

void file_enqueue_para_S(file_S *f, char *text, int size) {
  if (!f || !text) return;
  char sentence[1000];
  int j = 0;
  if (f->head == NULL) {
    f->head = f->tail = create_para_S();
  }
  para_S *p = f->tail;
  for (int i = 0; i < size; i++) {
    int next_i = i + 1;
    while (next_i < size && text[next_i] == '\r') next_i++;
    if (text[i] == '\n' && next_i < size && text[next_i] == '\n') {
      if (j > 0) {
        sentence[j] = '\0';
        char *start = sentence;
        while (*start == ' ') start++;
        if (*start != '\0') insert_S(para_get_trie_S(p), start, 1);
        j = 0;
      }
      para_S *temp = create_para_S();
      para_ass_adr_next_S(p, temp);
      para_ass_adr_prev_S(temp, p);
      p = temp;
      f->tail = p;
      i = next_i;
    } 
    else if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
      if (j > 0) {
        sentence[j] = '\0';
        char *start = sentence;
        while (*start == ' ') start++;
        if (*start != '\0') insert_S(para_get_trie_S(p), start, 1);
        j = 0;
      }
    } 
    else if (text[i] == '\n') {
      if (j < 999 && (j == 0 || sentence[j-1] != ' ')) sentence[j++] = ' ';
    } 
    else {
      if (j < 999) sentence[j++] = text[i];
    }
  }
  if (j > 0) {
    sentence[j] = '\0';
    char *start = sentence;
    while (*start == ' ') start++;
    if (*start != '\0') insert_S(para_get_trie_S(p), start, 1);
  }
}

void free_file_S(file_S *f) {
  if (!f)
    return;
  para_S *curr = f->head;
  while (curr) {
    para_S *next = curr->next;
    free_Trie_S(curr->trie);
    free(curr);
    curr = next;
  }
  free(f);
}

int file_is_empty_S(file_S *f) {
  if (f == NULL) return 1;
  return f->head == NULL;
}

Trie_S *file_get_trie_by_pos_S(file_S *f, int n) {
  if (file_is_empty_S(f)) return NULL;
  para_S *p = f->head;
  int i = 1;
  while (p != NULL && i < n) {
    p = para_get_next_S(p);
    i++;
  }
  return para_get_trie_S(p);
}

// Return the number of paragraphs in the queue.
int file_count_S(file_S *f) {
  if (f == NULL) return 0;
  para_S *p = f->head;
  int n = 0;
  while (p != NULL) {
    n++;
    p = para_get_next_S(p);
  }
  return n;
}


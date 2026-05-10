// queue (file) + linked_list (para) abstract machine

#include "../headers/file_structure.h"
#include "../headers/trie_machine_abstract.h"
#include <stdlib.h>

// paragraph-level functions
para *create_para() {
  para *p = malloc(sizeof(para));
  if (p == NULL)
    return NULL;
  p->trie = create_trie();
  p->next = p->prev = NULL;
  return p;
}

void para_ass_adr_trie(para *p, Trie *t) {
  if (p == NULL) return;
  p->trie = t;
  return;
}

void para_ass_adr_next(para *p, para *q) {
  if (p == NULL) return;
  p->next = q;
  return;
}

void para_ass_adr_prev(para *p, para *q) {
  if (p == NULL) return;
  p->prev = q;
  return;
}

Trie *para_get_trie(para *p) {
  if (p == NULL) return NULL;
  return p->trie;
}

para *para_get_next(para *p) {
  if (p == NULL) return NULL;
  return p->next;
}

para *para_get_prev(para *p) {
  if (p == NULL) return NULL;
  return p->prev;
}

// file-level functions
int file_is_empty(file *f) {
  if (f == NULL) return 1;
  return f->head == NULL;
}

Trie *file_get_trie_by_pos(file *f, int n) {      // 1-indexed 
  if (file_is_empty(f)) return NULL;             // returns NULL if n 
  para *p = f->head;                            // exceeds para count
  int i = 1;
  while (p != NULL && i < n) {
    p = para_get_next(p);
    i++;
  }
  return para_get_trie(p);
}

int file_count(file *f) {
  if (f == NULL) return 0;
  para *p = f->head;
  int n = 0;
  while (p != NULL) {
    n++;
    p = para_get_next(p);
  }
  return n;
}

file *create_file() {
  file *f = malloc(sizeof(file));
  if (f) {
    f->head = f->tail = NULL;
  }
  return f;
}

void free_file(file *file) {
  if (file == NULL) {
    return;
  }
  para *p = file->head;
  while (p != NULL) {
    para *nxt = p->next;
    free_Trie(para_get_trie(p));
    free(p);
    p = nxt;
  }
  free(file);
}

void file_enqueue_para(file *f, char *text, int nb) { 
  if (f == NULL || text == NULL) return;
  char word[1000];
  int j = 0;
  if (f->head == NULL) {
    f->head = f->tail = create_para();
  }
  para *p = f->tail;
  for (int i = 0; i < nb; i++) {
    int next_i = i + 1;
    while (next_i < nb && text[next_i] == '\r') next_i++;
    if (text[i] == '\n' && next_i < nb && text[next_i] == '\n') {  // paragraphs delimited by \n\n
      if (j > 0) {                                                
        word[j] = '\0';
        insert(para_get_trie(p), word, 1);
        j = 0;
      }
      para *temp = create_para();
      para_ass_adr_next(p, temp);
      para_ass_adr_prev(temp, p);
      p = temp;
      f->tail = p;
      i = next_i;
    } 
    else if (text[i] == ' ' || text[i] == '\n' || text[i] == '\t' ||      // sentence end chars
      text[i] == '.' || text[i] == '!' || text[i] == '?') { 
      if (j > 0) {
        word[j] = '\0';                    
        insert(para_get_trie(p), word, 1);     // copy over (insert) the temporary word to the trie
        j = 0;
      }
    } 
    else {
      if (j < 999) word[j++] = text[i];
    }
  }
  if (j > 0) {
    word[j] = '\0';
    insert(para_get_trie(p), word, 1);
  }
}

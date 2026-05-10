#ifndef FILE_STRUCTURE_H
#define FILE_STRUCTURE_H

#include "trie_machine_abstract.h"

typedef struct para {
  Trie *trie;
  struct para *next;
  struct para *prev;
} para;

typedef struct file {
  para *head;
  para *tail;
} file;

// paragraph-level functions
para *create_para();
void para_ass_adr_trie(para *p, Trie *t);
void para_ass_adr_next(para *p, para *q);
void para_ass_adr_prev(para *p, para *q);
Trie *para_get_trie(para *p);
para *para_get_next(para *p);
para *para_get_prev(para *p);

// file-level fucntions 
file *create_file();
void free_file(file *f);
void file_enqueue_para(file *f, char *text, int nb); 
int file_is_empty(file *f);
Trie *file_get_trie_by_pos(file *f, int n);
int file_count(file *f);

#endif // FILE_STRUCTURE_H

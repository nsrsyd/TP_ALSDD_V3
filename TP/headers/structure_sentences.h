#ifndef STRUCTURE_SENTENCES_H
#define STRUCTURE_SENTENCES_H

#include "trie_sentences.h"
#include "preprocess.h"

typedef struct para_S {
    Trie_S* trie;
    struct para_S* next;
    struct para_S* prev;
} para_S;

typedef struct file_S {
    para_S* head;
    para_S* tail;
} file_S;

// paragraph-level functions
para_S *create_para_S();
void para_ass_adr_trie_S(para_S *p, Trie_S *t);
void para_ass_adr_next_S(para_S *p, para_S *q);
void para_ass_adr_prev_S(para_S *p, para_S *q);
Trie_S *para_get_trie_S(para_S *p);
para_S *para_get_next_S(para_S *p);
para_S *para_get_prev_S(para_S *p);

// file-level fucntions 
file_S *create_file_S();
void file_enqueue_para_S(file_S* f, char* text, int size);
void free_file_S(file_S *f);
int file_is_empty_S(file_S *f);
Trie_S *file_get_trie_by_pos_S(file_S *f, int n);
int file_count_S(file_S *f);

#endif
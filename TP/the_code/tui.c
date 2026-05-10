#include "../headers/tui.h"
#include "../headers/file_structure.h"
#include "../headers/filtering_extraction.h"
#include "../headers/filtering_sentences.h"
#include "../headers/preprocess.h"
#include "../headers/sets_operations.h"
#include "../headers/sets_sentences.h"
#include "../headers/structure_sentences.h"
#include "../headers/trie_sentences.h"
#include "../headers/tui_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 10

typedef struct {
  char filename[256];
  file *par;
  Trie *combined_trie;
} LoadedFile;

LoadedFile loaded_files[MAX_FILES];
int loaded_file_count = 0;

typedef struct {
  char filename[256];
  file_S *sentence_file;
  Trie_S *combined_trie_S;
} LoadedSentenceFile;

LoadedSentenceFile loaded_sentences[MAX_FILES];
int sentence_file_count = 0;

char *read_file_to_string(const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (!f) {
    perror("Error opening file");
    return NULL;
  }
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *buf = malloc(size + 1);
  if (buf) {
    fread(buf, 1, size, f);
    buf[size] = '\0';
  }
  fclose(f);
  return buf;
}

Trie *merge_file_to_trie(file *par) {
  Trie *all_words = create_trie();
  para *curr = par->head;
  while (curr) {
    Trie *temp = union_a_b(all_words, curr->trie);
    free_Trie(all_words);
    all_words = temp;
    curr = curr->next;
  }
  return all_words;
}

void clean_up() {
  for (int i = 0; i < loaded_file_count; i++) {
    free_file(loaded_files[i].par);
    free_Trie(loaded_files[i].combined_trie);
  }
  loaded_file_count = 0;
}

Trie *select_trie_from_file(int idx) {
  int count = file_count(loaded_files[idx].par);
  printf("\n  Select target for file '%s':\n", loaded_files[idx].filename);
  printf("    [0] Entire file\n");
  for (int i = 1; i <= count; i++) {
    printf("    [%d] Paragraph %d\n", i, i);
  }
  int choice;
  printf("  Choice: ");
  if (scanf("%d", &choice) != 1) {
      while(getchar() != '\n');
      return loaded_files[idx].combined_trie;
  }
  if (choice >= 1 && choice <= count) {
    return file_get_trie_by_pos(loaded_files[idx].par, choice);
  }
  return loaded_files[idx].combined_trie;
}

Trie_S *select_trie_from_file_S(int idx) {
  int count = file_count_S(loaded_sentences[idx].sentence_file);
  printf("\n  Select target for file '%s':\n", loaded_sentences[idx].filename);
  printf("    [0] Entire file\n");
  for (int i = 1; i <= count; i++) {
    printf("    [%d] Paragraph %d\n", i, i);
  }
  int choice;
  printf("  Choice: ");
  if (scanf("%d", &choice) != 1) {
      while(getchar() != '\n');
      return loaded_sentences[idx].combined_trie_S;
  }
  if (choice >= 1 && choice <= count) {
    return file_get_trie_by_pos_S(loaded_sentences[idx].sentence_file, choice);
  }
  return loaded_sentences[idx].combined_trie_S;
}

void show_splash_screen() {
  clear_screen();
  printf("\n\n");
  printf("%s", COLOR_LIGHT_BLUE);
  typing_animation_centered(
      "  _______ _____  _____ ______    _______ ________   _________ ", 2);
  typing_animation_centered(
      " |__   __|  __ \\|_   _|  ____|  |__   __|  ____\\ \\ / /__   __|", 2);
  typing_animation_centered(
      "    | |  | |__) | | | | |__        | |  | |__   \\ V /   | |   ", 2);
  typing_animation_centered(
      "    | |  |  _  /  | | |  __|       | |  |  __|   > <    | |   ", 2);
  typing_animation_centered(
      "    | |  | | \\ \\ _| |_| |____      | |  | |____ / . \\   | |   ", 2);
  typing_animation_centered(
      "    |_|  |_|  \\_\\_____|______|     |_|  |______/_/ \\_\\  |_|   ", 2);
  printf("%s", COLOR_RESET);
  printf("\n");
  typing_animation_centered("Premium Text Analysis & Trie Management System",
                            10);
  printf("\n");
  draw_divider();
  print_centered("Initializing neural components...", COLOR_DIM);
  show_spinner(1000, "Loading Word Module");
  show_spinner(800, "Loading Sentence Module");
  show_spinner(500, "Finalizing UI");
  ms_sleep(1500);
  printf("\n");
}

void display_menu() {
  clear_screen();
  print_header("WORD MODE - MAIN MENU");

  printf("  %s[1]%s %-20s %s[2]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
         "Load a file", COLOR_YELLOW, COLOR_LIGHT_BLUE, "List loaded files");
  printf("  %s[3]%s %-20s %s[4]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
         "Set Union", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Set Intersection");
  printf("  %s[5]%s %-20s %s[6]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
         "Set Difference", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Check Similarity");
  printf("  %s[7]%s %-20s %s[8]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
         "Find File Topic", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Prefix Search");
  printf("  %s[9]%s %-20s %s[10]%s %-19s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
         "Count Matches", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Word Statistics");
  printf("  %s[0]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Back");
  printf("%s", COLOR_RESET);

  printf("\n");
  draw_divider();
  draw_footer(loaded_file_count, sentence_file_count);
  printf("\n");
  printf("  %sChoice:%s ", COLOR_BOLD, COLOR_RESET);
  fflush(stdout);
}

// Display-only: shows the file list without waiting for input
void show_file_list() {
  print_header("LOADED FILES");
  if (loaded_file_count == 0) {
    printf("  %sNo files loaded yet.%s\n", COLOR_DIM, COLOR_RESET);
    return;
  }
  for (int i = 0; i < loaded_file_count; i++) {
    printf("  %s[%d]%s %s%s%s\n", COLOR_YELLOW, i + 1, COLOR_RESET,
           COLOR_LIGHT_BLUE, loaded_files[i].filename, COLOR_RESET);
  }
  draw_divider();
}

// Standalone menu option: shows files and waits
void list_files() {
  show_file_list();
  press_any_key();
}

void load_file_flow() {
  if (loaded_file_count >= MAX_FILES) {
    printf("\n  %sMaximum files reached.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  char filename[256];
  printf("\n  Enter filename: ");
  scanf("%255s", filename);

  char *content = read_file_to_string(filename);
  if (!content) {
    ms_sleep(1000);
    return;
  }

  printf("  Preprocessing text...\n");
  to_lower_string(content);
  sanitize_text(content);

  file *par = create_file();
  file_enqueue_para(par, content, strlen(content));
  free(content);

  show_spinner(1500, "Analyzing Content");

  Trie *combined = merge_file_to_trie(par);

  strcpy(loaded_files[loaded_file_count].filename, filename);
  loaded_files[loaded_file_count].par = par;
  loaded_files[loaded_file_count].combined_trie = combined;
  loaded_file_count++;

  printf("\n");
  print_centered("File loaded successfully!", COLOR_YELLOW);
  ms_sleep(1000);
}

void set_operation_flow(int type) {
  if (loaded_file_count < 2) {
    printf("\n  %sNeed at least 2 files loaded.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  
  int k;
  printf("\n  How many targets to include? (K >= 2): ");
  if (scanf("%d", &k) != 1 || k < 2) {
      while(getchar() != '\n'); 
      k = 2;
  }

  show_file_list();
  Trie **tries = malloc(sizeof(Trie*) * k);
  for (int i = 0; i < k; i++) {
    int idx;
    printf("  Select file index for target %d: ", i + 1);
    if (scanf("%d", &idx) != 1 || idx < 1 || idx > loaded_file_count) {
       printf("  %sInvalid selection.%s\n", COLOR_YELLOW, COLOR_RESET);
       ms_sleep(1000);
       while(getchar() != '\n');
       free(tries);
       return;
    }
    tries[i] = select_trie_from_file(idx - 1);
  }

  Trie *result = NULL;
  char *op_name = "";

  switch (type) {
  case 1:
      result = union_a_b(tries[0], tries[1]);
      op_name = "Union";
      for (int i = 2; i < k; i++) {
          Trie *temp = union_a_b(result, tries[i]);
          free_Trie(result);
          result = temp;
      }
      break;
  case 2:
      result = intersection(tries[0], tries[1]);
      op_name = "Intersection";
      for (int i = 2; i < k; i++) {
          Trie *temp = intersection(result, tries[i]);
          free_Trie(result);
          result = temp;
      }
      break;
  case 3:
      result = difference(tries[0], tries[1]);
      op_name = "Difference";
      for (int i = 2; i < k; i++) {
          Trie *temp = difference(result, tries[i]);
          free_Trie(result);
          result = temp;
      }
      break;
  }

  free(tries);

  if (result) {
    printf("\n  %sResult of %s:%s\n", COLOR_LIGHT_BLUE, op_name, COLOR_RESET);
    draw_divider();
    print_words(result);
    draw_divider();
    printf("\n  Total words: %d\n", num_of_words(result));
    free_Trie(result);
    press_any_key();
  }
}

void similarity_flow() {
  if (loaded_file_count < 2) {
    printf("\n  %sNeed at least 2 files loaded.%s\n", COLOR_YELLOW,
           COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  show_file_list();
  int idx1, idx2;
  printf("  Select first file index: ");
  scanf("%d", &idx1);
  printf("  Select second file index: ");
  scanf("%d", &idx2);

  if (idx1 < 1 || idx1 > loaded_file_count || idx2 < 1 ||
      idx2 > loaded_file_count) {
    printf("  %sInvalid selection.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  Trie *t1 = select_trie_from_file(idx1);
  Trie *t2 = select_trie_from_file(idx2);

  Trie *copy_a = copy_trie(t1);
  Trie *copy_b = copy_trie(t2);
  int sim = check_similarity(copy_a, copy_b);
  free_Trie(copy_a);
  free_Trie(copy_b);
  printf("\n  %sSimilarity Profile:%s\n", COLOR_LIGHT_BLUE, COLOR_RESET);
  draw_divider();
  printf("  Jaccard Index (Word Level): %d%%\n", sim);
  draw_divider();
  press_any_key();
}

void find_topic_flow() {
  if (loaded_file_count == 0) {
    printf("\n  %sNo files loaded.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  show_file_list();
  int idx;
  printf("  Select file index: ");
  scanf("%d", &idx);

  if (idx < 1 || idx > loaded_file_count) {
    printf("  %sInvalid selection.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  idx--;

  Trie *copy_a = copy_trie(select_trie_from_file(idx));
  TopWords tw = topic(copy_a);
  free_Trie(copy_a);

  printf("\n");
  print_header("TOP 3 MOST USED WORDS");
  if (tw.count1 > 0) {
    printf("  %s#1%s  %-20s  (used %d times)\n", COLOR_YELLOW, COLOR_RESET,
           tw.word1, tw.count1);
  }
  if (tw.count2 > 0) {
    printf("  %s#2%s  %-20s  (used %d times)\n", COLOR_YELLOW, COLOR_RESET,
           tw.word2, tw.count2);
  }
  if (tw.count3 > 0) {
    printf("  %s#3%s  %-20s  (used %d times)\n", COLOR_YELLOW, COLOR_RESET,
           tw.word3, tw.count3);
  }
  if (tw.count1 == 0) {
    printf("  Could not identify any topic words.\n");
  }
  draw_divider();
  press_any_key();
}

void statistics_flow() {
  if (loaded_file_count == 0) {
    printf("\n  %sNo files loaded.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  show_file_list();
  int idx;
  printf("  Select file index: ");
  scanf("%d", &idx);

  if (idx < 1 || idx > loaded_file_count) {
    printf("  %sInvalid selection.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  idx--;

  printf("\n");
  print_header("WORD STATISTICS");
  Trie *selected = select_trie_from_file(idx);
  statistics(selected);
  printf("\n");
  draw_divider();
  printf("\n");
  print_header("PER-WORD BREAKDOWN");
  words_stats(selected);
  draw_divider();
  press_any_key();
}

void prefix_search_flow() {
  if (loaded_file_count == 0) {
    printf("\n  %sNo files loaded.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  show_file_list();
  int idx;
  printf("  Select file index: ");
  scanf("%d", &idx);

  if (idx < 1 || idx > loaded_file_count) {
    printf("  %sInvalid selection.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  idx--;

  char prefix[256];
  printf("  Enter prefix to search: ");
  scanf("%255s", prefix);

  char **results = words_start_with(select_trie_from_file(idx), prefix);
  if (results) {
    printf("\n  Words starting with '%s':\n", prefix);
    draw_divider();
    for (int i = 0; results[i] != NULL; i++) {
      printf("    - %s\n", results[i]);
      free(results[i]);
    }
    free(results);
  } else {
    printf("  No words found starting with '%s'.\n", prefix);
  }
  draw_divider();
  press_any_key();
}

void prefix_count_flow() {
  if (loaded_file_count == 0) {
    printf("\n  %sNo files loaded.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  show_file_list();
  int idx;
  printf("  Select file index: ");
  scanf("%d", &idx);

  if (idx < 1 || idx > loaded_file_count) {
    printf("  %sInvalid selection.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  idx--;

  char prefix[256];
  printf("  Enter prefix to count: ");
  scanf("%255s", prefix);

  int count = countPrefixMatches(select_trie_from_file(idx), prefix);
  printf("\n  %sStatistics:%s\n", COLOR_LIGHT_BLUE, COLOR_RESET);
  draw_divider();
  printf("  Matches for '%s': %d\n", prefix, count);
  draw_divider();
  press_any_key();
}

void start_word_tui() {
  int choice;
  do {
    display_menu();
    if (scanf("%d", &choice) != 1) {
      while (getchar() != '\n')
        ;
      continue;
    }

    switch (choice) {
    case 1:
      load_file_flow();
      break;
    case 2:
      list_files();
      break;
    case 3:
      set_operation_flow(1);
      break;
    case 4:
      set_operation_flow(2);
      break;
    case 5:
      set_operation_flow(3);
      break;
    case 6:
      similarity_flow();
      break;
    case 7:
      find_topic_flow();
      break;
    case 8:
      prefix_search_flow();
      break;
    case 9:
      prefix_count_flow();
      break;
    case 10:
      statistics_flow();
      break;
    case 0:
      printf("\n");
      print_centered("Returning to mode selection...", COLOR_DIM);
      ms_sleep(500);
      break;
    default:
      printf("  %sInvalid choice.%s\n", COLOR_YELLOW, COLOR_RESET);
      ms_sleep(800);
      break;
    }
  } while (choice != 0);
}

// --- Sentence Mode Flows ---

// Display-only: shows sentence file list without waiting
void show_sentence_file_list() {
  print_header("LOADED SENTENCE FILES");
  if (sentence_file_count == 0) {
    printf("  %sNo sentence files loaded yet.%s\n", COLOR_DIM, COLOR_RESET);
    return;
  }
  for (int i = 0; i < sentence_file_count; i++) {
    printf("  %s[%d]%s %s%s%s\n", COLOR_YELLOW, i + 1, COLOR_RESET,
           COLOR_LIGHT_BLUE, loaded_sentences[i].filename, COLOR_RESET);
  }
  draw_divider();
}

// Standalone menu option: shows sentence files and waits
void list_sentences_files() {
  show_sentence_file_list();
  press_any_key();
}

void load_sentence_file_flow() {
  if (sentence_file_count >= MAX_FILES) {
    printf("\n  %sMaximum files reached.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  char filename[256];
  printf("\n  Enter filename: ");
  scanf("%255s", filename);

  char *content = read_file_to_string(filename);
  if (!content) {
    ms_sleep(1000);
    return;
  }

  show_spinner(1500, "Analyzing Content");
  to_lower_string(content);
  sanitize_text(content);
  file_S *fs = create_file_S();
  file_enqueue_para_S(fs, content, strlen(content));
  free(content);

  Trie_S *combined = create_trie_S();
  para_S *curr = fs->head;
  while (curr) {
    Trie_S *temp = union_a_b_S(combined, para_get_trie_S(curr));
    free_Trie_S(combined);
    combined = temp;
    curr = para_get_next_S(curr);
  }

  strcpy(loaded_sentences[sentence_file_count].filename, filename);
  loaded_sentences[sentence_file_count].sentence_file = fs;
  loaded_sentences[sentence_file_count].combined_trie_S = combined;
  sentence_file_count++;

  printf("\n");
  print_centered("Sentence file loaded successfully!", COLOR_YELLOW);
  ms_sleep(1000);
}

void set_operation_sentence_flow(int type) {
  if (sentence_file_count < 2) {
    printf("\n  %sNeed at least 2 sentence files loaded.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  
  int k;
  printf("\n  How many targets to include? (K >= 2): ");
  if (scanf("%d", &k) != 1 || k < 2) {
      while(getchar() != '\n'); 
      k = 2;
  }

  show_sentence_file_list();
  Trie_S **tries = malloc(sizeof(Trie_S*) * k);
  for (int i = 0; i < k; i++) {
    int idx;
    printf("  Select file index for target %d: ", i + 1);
    if (scanf("%d", &idx) != 1 || idx < 1 || idx > sentence_file_count) {
       printf("  %sInvalid selection.%s\n", COLOR_YELLOW, COLOR_RESET);
       ms_sleep(1000);
       while(getchar() != '\n');
       free(tries);
       return;
    }
    tries[i] = select_trie_from_file_S(idx - 1);
  }

  Trie_S *result = NULL;
  char *op_name = "";

  switch (type) {
  case 1:
      result = union_a_b_S(tries[0], tries[1]);
      op_name = "Union";
      for (int i = 2; i < k; i++) {
          Trie_S *temp = union_a_b_S(result, tries[i]);
          free_Trie_S(result);
          result = temp;
      }
      break;
  case 2:
      result = intersection_S(tries[0], tries[1]);
      op_name = "Intersection";
      for (int i = 2; i < k; i++) {
          Trie_S *temp = intersection_S(result, tries[i]);
          free_Trie_S(result);
          result = temp;
      }
      break;
  case 3:
      result = difference_S(tries[0], tries[1]);
      op_name = "Difference";
      for (int i = 2; i < k; i++) {
          Trie_S *temp = difference_S(result, tries[i]);
          free_Trie_S(result);
          result = temp;
      }
      break;
  }

  free(tries);

  if (result) {
    printf("\n  %sResult of Sentence %s:%s\n", COLOR_LIGHT_BLUE, op_name,
           COLOR_RESET);
    print_sentences_S(result);
    printf("\n  Total sentences: %d\n", num_of_sentences_S(result));
    free_Trie_S(result);
    press_any_key();
  }
}

void similarity_sentence_flow() {
  if (sentence_file_count < 2) {
    printf("\n  %sNeed at least 2 sentence files loaded.%s\n", COLOR_YELLOW,
           COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  show_sentence_file_list();
  int idx1, idx2;
  printf("  Select first file index: ");
  scanf("%d", &idx1);
  printf("  Select second file index: ");
  scanf("%d", &idx2);

  if (idx1 < 1 || idx1 > sentence_file_count || idx2 < 1 ||
      idx2 > sentence_file_count) {
    printf("  %sInvalid selection.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  Trie_S *t1 = select_trie_from_file_S(idx1);
  Trie_S *t2 = select_trie_from_file_S(idx2);

  double sim = calculateJaccard_S(t1, t2);
  printf("\n  Sentence Similarity (Jaccard): %.2f%%\n", sim * 100);
  draw_divider();
  press_any_key();
}

void prefix_search_sentence_flow() {
  if (sentence_file_count == 0) {
    printf("\n  %sNo sentence files loaded.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  show_sentence_file_list();
  int idx;
  printf("  Select file index: ");
  scanf("%d", &idx);

  if (idx < 1 || idx > sentence_file_count) {
    printf("  %sInvalid selection.%s\n", COLOR_YELLOW, COLOR_RESET);
    ms_sleep(1000);
    return;
  }
  idx--;

  char prefix[256];
  printf("  Enter sentence prefix (e.g. 'the computer'): ");
  getchar(); // clear leftover \n
  fgets(prefix, sizeof(prefix), stdin);
  prefix[strcspn(prefix, "\n")] = 0;

  char **results =
      sentences_start_with_S(select_trie_from_file_S(idx), prefix);
  if (results) {
    printf("\n  Sentences starting with '%s':\n", prefix);
    draw_divider();
    for (int i = 0; results[i] != NULL; i++) {
      printf("    - %s\n", results[i]);
      free(results[i]);
    }
    free(results);
  } else {
    printf("  No sentences found starting with '%s'.\n", prefix);
  }
  draw_divider();
  press_any_key();
}

void clean_up_sentences() {
  for (int i = 0; i < sentence_file_count; i++) {
    free_file_S(loaded_sentences[i].sentence_file);
    free_Trie_S(loaded_sentences[i].combined_trie_S);
  }
  sentence_file_count = 0;
}

void start_sentence_tui() {
  int choice;
  do {
    clear_screen();
    print_header("SENTENCE MODE - MAIN MENU");

    printf("  %s[1]%s %-20s %s[2]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
           "Load file", COLOR_YELLOW, COLOR_LIGHT_BLUE, "List files");
    printf("  %s[3]%s %-20s %s[4]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
           "Union", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Intersection");
    printf("  %s[5]%s %-20s %s[6]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
           "Difference", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Similarity");
    printf("  %s[7]%s %-20s %s[0]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
           "Prefix Search", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Back");
    printf("%s", COLOR_RESET);

    printf("\n");
    draw_divider();
    draw_footer(loaded_file_count, sentence_file_count);
    printf("\n");
    printf("  %sChoice:%s ", COLOR_BOLD, COLOR_RESET);
    fflush(stdout);

    if (scanf("%d", &choice) != 1) {
      while (getchar() != '\n')
        ;
      continue;
    }

    switch (choice) {
    case 1:
      load_sentence_file_flow();
      break;
    case 2:
      list_sentences_files();
      break;
    case 3:
      set_operation_sentence_flow(1);
      break;
    case 4:
      set_operation_sentence_flow(2);
      break;
    case 5:
      set_operation_sentence_flow(3);
      break;
    case 6:
      similarity_sentence_flow();
      break;
    case 7:
      prefix_search_sentence_flow();
      break;
    case 0:
      printf("\n");
      print_centered("Returning to mode selection...", COLOR_DIM);
      ms_sleep(500);
      break;
    default:
      printf("  %sInvalid choice.%s\n", COLOR_YELLOW, COLOR_RESET);
      ms_sleep(800);
      break;
    }
  } while (choice != 0);
}

void start_tui() {
  show_splash_screen();
  int mode;
  do {
    clear_screen();
    print_header("MODE SELECTION");

    printf("  %s[1]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Word Mode");
    printf("  %s[2]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE,
           "Sentence Mode");
    printf("  %s[0]%s %-20s\n", COLOR_YELLOW, COLOR_LIGHT_BLUE, "Exit Program");
    printf("%s", COLOR_RESET);

    printf("\n");
    draw_divider();
    draw_footer(loaded_file_count, sentence_file_count);
    printf("\n");
    printf("  %sChoice:%s ", COLOR_BOLD, COLOR_RESET);
    fflush(stdout);

    if (scanf("%d", &mode) != 1) {
      while (getchar() != '\n')
        ;
      continue;
    }

    if (mode == 1) {
      start_word_tui();
    } else if (mode == 2) {
      start_sentence_tui();
    } else if (mode != 0) {
      printf("  %sInvalid mode selection.%s\n", COLOR_YELLOW, COLOR_RESET);
      ms_sleep(800);
    }
  } while (mode != 0);

  clean_up();
  clean_up_sentences();

  clear_screen();
  printf("\n");
  print_centered("Thank you for using Trie Text System. Goodbye!",
                 COLOR_LIGHT_BLUE);
  printf("\n");
}

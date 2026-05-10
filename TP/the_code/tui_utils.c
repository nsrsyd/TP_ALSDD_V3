#include "../headers/tui_utils.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void clear_screen() {
#ifdef _WIN32
  system("cls");
#else
  printf("\033[H\033[J");
#endif
}

void move_cursor(int x, int y) { printf("\033[%d;%dH", y, x); }

void hide_cursor() { printf("\033[?25l"); }

void show_cursor() { printf("\033[?25h"); }

int get_terminal_width() {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int columns;
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
  }
  return 80;
#else
  return 80;
#endif
}

void print_centered(const char *text, const char *color) {
  int width = get_terminal_width();
  int len = strlen(text);
  int padding = (width - len) / 2;
  if (padding < 0)
    padding = 0;

  printf("%s", color);
  for (int i = 0; i < padding; i++)
    putchar(' ');
  printf("%s%s\n", text, COLOR_RESET);
}

void print_centered_no_newline(const char *text, const char *color) {
  int width = get_terminal_width();
  int len = strlen(text);
  int padding = (width - len) / 2;
  if (padding < 0)
    padding = 0;

  printf("%s", color);
  for (int i = 0; i < padding; i++)
    putchar(' ');
  printf("%s%s", text, COLOR_RESET);
  fflush(stdout);
}

void ms_sleep(int ms) {
#ifdef _WIN32
  Sleep(ms);
#else
  usleep(ms * 1000);
#endif
}

void typing_animation(const char *text, int delay_ms) {
  for (int i = 0; text[i] != '\0'; i++) {
    putchar(text[i]);
    fflush(stdout);
    ms_sleep(delay_ms);
  }
}

void typing_animation_centered(const char *text, int delay_ms) {
  int width = get_terminal_width();
  int len = strlen(text);
  int padding = (width - len) / 2;
  if (padding < 0)
    padding = 0;

  for (int i = 0; i < padding; i++)
    putchar(' ');

  for (int i = 0; text[i] != '\0'; i++) {
    putchar(text[i]);
    fflush(stdout);
    ms_sleep(delay_ms);
  }
  printf("\n");
}

void print_header(const char *text) {
  int width = get_terminal_width();
  int len = strlen(text);
  int total_len = len + 6;
  int padding = (width - total_len) / 2;
  if (padding < 0)
    padding = 0;

  printf("\n");
  for (int i = 0; i < padding; i++)
    putchar(' ');
  printf("%s", COLOR_LIGHT_BLUE);
  printf("  %s%s%s  \n", COLOR_BOLD, text, COLOR_RESET);

  for (int i = 0; i < padding; i++)
    putchar(' ');
  printf("%s", COLOR_YELLOW);
  for (int i = 0; i < total_len; i++)
    printf("=");
  printf("%s\n\n", COLOR_RESET);
}

void show_spinner(int duration_ms, const char *message) {
  const char *chars = "|/-\\";
  int iterations = duration_ms / 100;
  int width = get_terminal_width();
  int msg_len = strlen(message) + 10;
  int padding = (width - msg_len) / 2;
  if (padding < 0)
    padding = 0;

  hide_cursor();
  for (int i = 0; i < iterations; i++) {
    printf("\r");
    for (int j = 0; j < padding; j++)
      putchar(' ');
    printf("%s [%c] %s", COLOR_LIGHT_BLUE, chars[i % 4], message);
    printf("%s", COLOR_RESET);
    fflush(stdout);
    ms_sleep(100);
  }
  printf("\r");
  for (int j = 0; j < padding; j++)
    putchar(' ');
  printf("%s [OK] %s\n", COLOR_YELLOW, message);
  printf("%s", COLOR_RESET);
  show_cursor();
}

void draw_divider() {
  int width = get_terminal_width();
  printf("%s", COLOR_DIM);
  for (int i = 0; i < width; i++)
    printf("-");
  printf("%s\n", COLOR_RESET);
}

void draw_footer(int word_file_count, int sentence_file_count) {
  int width = get_terminal_width();
  printf("%s", COLOR_CYAN);
  for (int i = 0; i < width; i++)
    printf("=");
  printf("\n");

  char status[256];
  sprintf(status, " Word Files: %d | Sentence Files: %d | System: Ready ",
          word_file_count, sentence_file_count);

  int len = strlen(status);
  int padding = (width - len) / 2;
  if (padding < 0)
    padding = 0;

  for (int i = 0; i < padding; i++)
    putchar(' ');
  printf("%s%s\n", status, COLOR_RESET);
}

void press_any_key() {
  // Drain any leftover characters in stdin (e.g. from scanf)
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
  // Now prompt and wait for a fresh Enter
  print_centered("Press [Enter] to continue...", COLOR_DIM);
  getchar();
}

#ifndef TUI_UTILS_H
#define TUI_UTILS_H

#include <stdio.h>

// ANSI Escape Codes for Colors
#define COLOR_LIGHT_BLUE "\033[1;36m"
#define COLOR_YELLOW     "\033[1;33m"
#define COLOR_RESET      "\033[0m"
#define COLOR_BOLD       "\033[1m"
#define COLOR_DIM        "\033[2m"
#define COLOR_CYAN       "\033[0;36m"

// Utility Functions
void clear_screen();
void move_cursor(int x, int y);
void hide_cursor();
void show_cursor();

void ms_sleep(int ms);
int get_terminal_width();
void print_centered(const char *text, const char *color);
void print_centered_no_newline(const char *text, const char *color);
void print_boxed(const char *text, int padding, const char *color);
void print_header(const char *text);
void typing_animation_centered(const char *text, int delay_ms);
void show_spinner(int duration_ms, const char *message);
void draw_divider();
void draw_footer(int word_file_count, int sentence_file_count);
void press_any_key();

#endif // TUI_UTILS_H

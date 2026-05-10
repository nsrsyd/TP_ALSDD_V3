#include "../headers/preprocess.h"
#include <ctype.h>
#include <stdlib.h>


void to_lower_string(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void sanitize_text(char *str) {
    if (str == NULL) return;
    int i = 0, j = 0;
    while (str[i]) {
        // preserve sentence delimiters and newlines; collapse everything else
        if (isalpha((unsigned char)str[i]) || isspace((unsigned char)str[i]) ||
    str[i] == '.' || str[i] == '!' || str[i] == '?') {
            str[j++] = str[i];
        } 
        else  str[j++] = ' ';
        i++;
    }
    str[j] = '\0';
}

#ifndef PREPROCESS_H
#define PREPROCESS_H

/**
 * @brief Converts all characters in a string to lowercase.
 * 
 * @param str The string to convert.
 */
void to_lower_string(char *str);

/**
 * @brief Removes non-alphabetic characters (except spaces and newlines) 
 * from a string to make it compatible with the Trie implementation.
 * 
 * @param str The string to sanitize.
 */
void sanitize_text(char *str);

#endif // PREPROCESS_H

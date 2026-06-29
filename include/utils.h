#pragma once

#include <stddef.h>

unsigned int string_length(const char *str);
bool strstart(const char *str1, const char *str2);
char* strip_prefix(const char *str1, const char *str2);
size_t split_whitespace_inplace(char *str, size_t max_words, char **out_words);
int strcmp(const char *str1, const char *str2);

#include "utils.h"
#include "io.h"
#include <stdbool.h>
#include <stddef.h>

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

bool strstart(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    return (*str2 == '\0');
}

char* strip_prefix(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    const char* new_char = str1;
    return (char*)new_char;
}

unsigned int string_length(const char *str) {
    unsigned int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}


static bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}
size_t split_whitespace_inplace(char *str, size_t max_words, char **out_words) {
    if (str == NULL || out_words == NULL || max_words == 0) return 0;

    size_t word_count = 0;
    bool in_word = false;

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!is_space(str[i])) {
            if (!in_word) {
                if (word_count >= max_words) break; 
                
                out_words[word_count] = &str[i];
                word_count++;
                in_word = true;
            }
        } else {
            if (in_word) {
                str[i] = '\0';
                in_word = false;
            }
        }
    }

    return word_count;
}

void print_int(unsigned int value) {
    char buffer[11];
    int i = 9;
    buffer[10] = '\0';
    
    if (value == 0) {
        print_string("0");
        return;
    }
    
    while (value > 0 && i >= 0) {
        buffer[i] = (value % 10) + '0';
        value /= 10;
        i--;
    }
    print_string(&buffer[i + 1]);
}


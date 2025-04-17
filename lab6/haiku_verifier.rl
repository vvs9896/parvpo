#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

%%{
    machine HaikuVerifier;
    
    action mark_line_start { line_start = fpc; }
    action check_first_line {
        int syllables = count_syllables(line_start, fpc);
        if (syllables != 5) {
            first_line_valid = false;
            printf("First line: %d syllables (should be 5)\n", syllables);
        } else {
            first_line_valid = true;
        }
    }
    
    action check_second_line {
        int syllables = count_syllables(line_start, fpc);
        if (syllables != 7) {
            second_line_valid = false;
            printf("Second line: %d syllables (should be 7)\n", syllables);
        } else {
            second_line_valid = true;
        }
    }
    
    action check_third_line {
        int syllables = count_syllables(line_start, fpc);
        if (syllables != 5) {
            third_line_valid = false;
            printf("Third line: %d syllables (should be 5)\n", syllables);
        } else {
            third_line_valid = true;
        }
    }
    
    any_char = any - '\n';
    
    line = any_char* >mark_line_start;
    
    haiku = 
        line '\n' @check_first_line
        line '\n' @check_second_line
        line '\n' @check_third_line
        '\n'?;
    
    main := haiku @{ is_valid_haiku = true; } $err{ is_valid_haiku = false; };
}%%

%% write data;

typedef struct {
    const char* word;
    int syllables;
} SyllableDict;

static const SyllableDict dictionary[] = {
    {"beautiful", 3}, {"wonderful", 3}, {"happiness", 3}, 
    {"tomorrow", 3}, {"mysterious", 4}, {"universe", 3},
    {"waterfall", 3}, {"eternity", 4}, {"loneliness", 3}, 
    {"memory", 3}, {"glorious", 3}, {"horizon", 3},
    {"melody", 3}, {"radiant", 3}, {"symphony", 3},
    {"harmony", 3}, {"wandering", 3}, {"tranquil", 2}, 
    {"peaceful", 2}, {"ancestor", 3}, {"water", 2},
    {"falling", 2}, {"sunset", 2}, {"morning", 2}, 
    {"flowers", 2}, {"mountain", 2}, {"darkness", 2}, 
    {"silence", 2}, {"winter", 2}, {"summer", 2},
    {"river", 2}, {"forest", 2}, {"meadow", 2}, 
    {"autumn", 2}, {"springtime", 2}, {"pebble", 2}, 
    {"willow", 2}, {"open", 2}, {"gentle", 2}, 
    {"whisper", 2}, {"silent", 2}, {"poetry", 3},
    {"cat", 1}, {"dog", 1}, {"sun", 1}, {"moon", 1}, 
    {"night", 1}, {"day", 1}, {"rain", 1}, {"snow", 1}, 
    {"tree", 1}, {"bird", 1}, {"fish", 1}, {"star", 1}, 
    {"sky", 1}, {"cloud", 1}, {"breeze", 1}, {"wind", 1}, 
    {"sea", 1}, {"lake", 1}, {"hill", 1}, {"stone", 1},
    {"nature's", 2}, {"cherry", 2}, {"blossoms", 2},
    {"petals", 2}, {"through", 1}, {"spring", 1},
    {"beauty", 2}, {"decay", 2}, {"shadows", 2},
    {"dancing", 2}, {"peaceful", 2}, {"descends", 2},
    {"moonlight", 2}, {"gleams", 1}, {"whispering", 3},
    {"in", 1}, {"the", 1}, {"air", 1}, {"on", 1},
    {"pink", 1}, {"drift", 1}
};

#define DICT_SIZE (sizeof(dictionary) / sizeof(SyllableDict))

int find_in_dictionary(const char* word) {
    char word_lower[50] = {0};
    int len = strlen(word);
    if (len >= sizeof(word_lower)) len = sizeof(word_lower) - 1;
    
    for (int i = 0; i < len; i++) {
        word_lower[i] = tolower(word[i]);
    }
    word_lower[len] = '\0';
    
    for (size_t i = 0; i < DICT_SIZE; i++) {
        if (strcmp(dictionary[i].word, word_lower) == 0) {
            return dictionary[i].syllables;
        }
    }
    
    return -1;
}

bool is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y');
}

int count_syllables_algo(const char* word) {
    int count = 0;
    bool prev_is_vowel = false;
    int len = strlen(word);
    
    for (int i = 0; i < len; i++) {
        if (!isalpha(word[i])) continue;
        
        char c = tolower(word[i]);
        bool current_is_vowel = is_vowel(c);
        
        if (current_is_vowel && !prev_is_vowel) {
            count++;
        }
        
        if (current_is_vowel && prev_is_vowel) {
            if (i > 0) {
                char prev = tolower(word[i-1]);
                if ((prev == 'a' && (c == 'i' || c == 'y' || c == 'u')) ||
                    (prev == 'e' && (c == 'a' || c == 'e' || c == 'i' || c == 'y')) ||
                    (prev == 'o' && (c == 'a' || c == 'i' || c == 'y' || c == 'u')) ||
                    (prev == 'o' && c == 'o') ||
                    (prev == 'u' && c == 'i')) {
                    count--;
                }
            }
        }
        
        prev_is_vowel = current_is_vowel;
    }
    
    if (len > 2 && tolower(word[len-1]) == 'e') {
        if (tolower(word[len-2]) == 'l' && len > 3 && !is_vowel(word[len-3])) {
        }
        else if (count > 1 && !is_vowel(word[len-2])) {
            count--;
        }
    }
    
    if (len > 2 && tolower(word[len-1]) == 'd' && tolower(word[len-2]) == 'e') {
        if (count > 1 && len > 3 && !is_vowel(word[len-3])) {
            count--;
        }
    }
    
    if (count == 0 && len > 0) {
        count = 1;
    }
    
    return count;
}

int count_syllables(const char* start, const char* end) {
    int total_syllables = 0;
    char word[100];
    int word_len = 0;
    
    const char* p = start;
    while (p < end) {
        if (isalpha(*p) || *p == '\'') {
            if (word_len < 99) {
                word[word_len++] = *p;
            }
        } else if (word_len > 0) {
            word[word_len] = '\0';
            
            int syllables = find_in_dictionary(word);
            if (syllables < 0) {
                syllables = count_syllables_algo(word);
            }
            
            total_syllables += syllables;
            word_len = 0;
        }
        p++;
    }
    
    if (word_len > 0) {
        word[word_len] = '\0';
        int syllables = find_in_dictionary(word);
        if (syllables < 0) {
            syllables = count_syllables_algo(word);
        }
        total_syllables += syllables;
    }
    
    return total_syllables;
}

bool verify_haiku(const char *input) {
    int cs;
    bool is_valid_haiku = false;
    bool first_line_valid = false;
    bool second_line_valid = false;
    bool third_line_valid = false;
    const char *line_start = NULL;
    
    const char *p = input;
    const char *pe = p + strlen(p);
    const char *eof = pe;
    
    %% write init;
    %% write exec;
    
    return is_valid_haiku && first_line_valid && second_line_valid && third_line_valid;
}

int count_syllables_text(const char* text) {
    return count_syllables(text, text + strlen(text));
}

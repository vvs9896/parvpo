#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

bool verify_haiku(const char *input);
int count_syllables_text(const char *text);

typedef struct {
    const char* word;
    int syllables;
} Word;

Word dict_1syllable[] = {
    {"cat", 1}, {"dog", 1}, {"sun", 1}, {"moon", 1}, {"night", 1},
    {"day", 1}, {"rain", 1}, {"snow", 1}, {"tree", 1}, {"bird", 1},
    {"fish", 1}, {"star", 1}, {"sky", 1}, {"cloud", 1}, {"breeze", 1},
    {"wind", 1}, {"sea", 1}, {"lake", 1}, {"hill", 1}, {"stone", 1},
    {"in", 1}, {"the", 1}, {"on", 1}, {"through", 1}, {"spring", 1},
    {"gleams", 1}, {"air", 1}, {"drift", 1}, {"pink", 1}
};

Word dict_2syllable[] = {
    {"water", 2}, {"falling", 2}, {"sunset", 2}, {"morning", 2}, {"flowers", 2},
    {"mountain", 2}, {"darkness", 2}, {"silence", 2}, {"winter", 2}, {"summer", 2},
    {"river", 2}, {"forest", 2}, {"meadow", 2}, {"autumn", 2}, {"springtime", 2},
    {"pebble", 2}, {"willow", 2}, {"open", 2}, {"gentle", 2}, {"whisper", 2},
    {"silent", 2}, {"nature's", 2}, {"cherry", 2}, {"blossoms", 2}, {"petals", 2},
    {"beauty", 2}, {"decay", 2}, {"shadows", 2}, {"dancing", 2}, {"peaceful", 2},
    {"descends", 2}, {"moonlight", 2}
};

Word dict_3syllable[] = {
    {"beautiful", 3}, {"wonderful", 3}, {"happiness", 3}, {"tomorrow", 3},
    {"universe", 3}, {"waterfall", 3}, {"memory", 3}, {"glorious", 3},
    {"horizon", 3}, {"melody", 3}, {"radiant", 3}, {"symphony", 3},
    {"harmony", 3}, {"wandering", 3}, {"poetry", 3}, {"whispering", 3}
};

Word dict_4syllable[] = {
    {"mysterious", 4}, {"eternity", 4}
};

#define DICT_1SYL_SIZE (sizeof(dict_1syllable) / sizeof(Word))
#define DICT_2SYL_SIZE (sizeof(dict_2syllable) / sizeof(Word))
#define DICT_3SYL_SIZE (sizeof(dict_3syllable) / sizeof(Word))
#define DICT_4SYL_SIZE (sizeof(dict_4syllable) / sizeof(Word))

bool validate_line(const char* line, int expected_syllables) {
    char* temp = strdup(line);
    char* newline = strchr(temp, '\n');
    if (newline) *newline = '\0';
    
    int actual = count_syllables_text(temp);
    bool valid = (actual == expected_syllables);
    
    if (!valid) {
        printf("Validation error: '%s' has %d syllables, expected %d\n", 
               temp, actual, expected_syllables);
    }
    
    free(temp);
    return valid;
}

const char* get_random_word(int syllables) {
    Word* dict;
    size_t dict_size;
    
    switch (syllables) {
        case 1:
            dict = dict_1syllable;
            dict_size = DICT_1SYL_SIZE;
            break;
        case 2:
            dict = dict_2syllable;
            dict_size = DICT_2SYL_SIZE;
            break;
        case 3:
            dict = dict_3syllable;
            dict_size = DICT_3SYL_SIZE;
            break;
        case 4:
            dict = dict_4syllable;
            dict_size = DICT_4SYL_SIZE;
            break;
        default:
            return NULL;
    }
    
    return dict[rand() % dict_size].word;
}

char* generate_line(int target_syllables) {
    char* line = (char*)malloc(100);
    if (!line) return NULL;
    line[0] = '\0';
    
    int current_syllables = 0;
    
    while (current_syllables < target_syllables) {
        int syllables_needed = target_syllables - current_syllables;
        int syllables_to_add;
        
        if (syllables_needed >= 4) {
            int options[] = {1, 2, 3, 4};
            syllables_to_add = options[rand() % 4];
        } else {
            syllables_to_add = syllables_needed;
        }
        
        const char* word = get_random_word(syllables_to_add);
        if (word == NULL) continue;
        
        if (current_syllables > 0) {
            strcat(line, " ");
        }
        strcat(line, word);
        current_syllables += syllables_to_add;
    }
    
    strcat(line, "\n");
    return line;
}

char* generate_random_haiku() {
    char* haiku = (char*)malloc(300);
    if (!haiku) return NULL;
    haiku[0] = '\0';
    
    char* line1 = generate_line(5);
    if (!validate_line(line1, 5)) {
        printf("First line validation failed\n");
    }
    
    char* line2 = generate_line(7);
    if (!validate_line(line2, 7)) {
        printf("Second line validation failed\n");
    }
    
    char* line3 = generate_line(5);
    if (!validate_line(line3, 5)) {
        printf("Third line validation failed\n");
    }
    
    strcat(haiku, line1);
    strcat(haiku, line2);
    strcat(haiku, line3);
    strcat(haiku, "\n");
    
    free(line1);
    free(line2);
    free(line3);
    
    return haiku;
}

char* generate_invalid_haiku() {
    if (rand() % 2 == 0) {
        char* haiku = (char*)malloc(300);
        if (!haiku) return NULL;
        haiku[0] = '\0';
        
        int invalid_line = rand() % 3;
        
        if (invalid_line == 0) {
            int syllables = (rand() % 2 == 0) ? 4 : 6;
            char* line1 = generate_line(syllables);
            char* line2 = generate_line(7);
            char* line3 = generate_line(5);
            
            strcat(haiku, line1);
            strcat(haiku, line2);
            strcat(haiku, line3);
            strcat(haiku, "\n");
            
            free(line1);
            free(line2);
            free(line3);
        } else if (invalid_line == 1) {
            char* line1 = generate_line(5);
            int syllables = (rand() % 2 == 0) ? 6 : 8;
            char* line2 = generate_line(syllables);
            char* line3 = generate_line(5);
            
            strcat(haiku, line1);
            strcat(haiku, line2);
            strcat(haiku, line3);
            strcat(haiku, "\n");
            
            free(line1);
            free(line2);
            free(line3);
        } else {
            char* line1 = generate_line(5);
            char* line2 = generate_line(7);
            int syllables = (rand() % 2 == 0) ? 4 : 6;
            char* line3 = generate_line(syllables);
            
            strcat(haiku, line1);
            strcat(haiku, line2);
            strcat(haiku, line3);
            strcat(haiku, "\n");
            
            free(line1);
            free(line2);
            free(line3);
        }
        
        return haiku;
    } else {
        char* haiku = generate_random_haiku();
        if (!haiku) return NULL;
        
        int error_type = rand() % 3;
        switch (error_type) {
            case 0:
                {
                    char* pos = strchr(haiku, '\n');
                    if (pos) {
                        char* next_pos = strchr(pos + 1, '\n');
                        if (next_pos) {
                            strcpy(pos, next_pos);
                        }
                    }
                }
                break;
            case 1:
                {
                    char* extra_line = generate_line(rand() % 5 + 3);
                    strcat(haiku, extra_line);
                    free(extra_line);
                }
                break;
            case 2:
                {
                    size_t len = strlen(haiku);
                    if (len >= 2 && haiku[len-1] == '\n' && haiku[len-2] == '\n') {
                        haiku[len-1] = '\0';
                    }
                }
                break;
        }
        
        return haiku;
    }
}

void print_syllable_info(const char* haiku) {
    printf("Syllable analysis:\n");
    
    const char* start = haiku;
    const char* end;
    int line_num = 1;
    
    while ((end = strchr(start, '\n')) != NULL) {
        char line[100];
        int len = end - start;
        if (len > 0 && len < 100) {
            strncpy(line, start, len);
            line[len] = '\0';
            
            if (strlen(line) > 0) {
                int syllables = count_syllables_text(line);
                printf("Line %d (\"%s\"): %d syllables\n", line_num, line, syllables);
                line_num++;
            }
        }
        
        start = end + 1;
    }
}

void print_verification_result(const char* haiku, bool is_valid) {
    printf("\n----------------------------\n");
    printf("Haiku text:\n%s\n", haiku);
    print_syllable_info(haiku);
    printf("Verification result: %s\n", is_valid ? "VALID" : "INVALID");
    printf("----------------------------\n");
}

int main() {
    srand(time(NULL));
    
    printf("\n=== Testing Syllable Counter ===\n");
    const char* test_words[] = {
        "cat", "dog", "beautiful", "waterfall", "happiness", 
        "rain", "mountain", "mysterious", "silent", "poetry",
        "nature's", "cherry", "blossoms", "petals", "through"
    };
    
    for (int i = 0; i < 15; i++) {
        int syllables = count_syllables_text(test_words[i]);
        printf("Word: \"%s\" - Syllables: %d\n", test_words[i], syllables);
    }
    
    printf("\n=== Verifying Valid Haikus ===\n");
    for (int i = 0; i < 3; i++) {
        char* valid_haiku = generate_random_haiku();
        bool is_valid = verify_haiku(valid_haiku);
        print_verification_result(valid_haiku, is_valid);
        free(valid_haiku);
    }
    
    printf("\n=== Verifying Invalid Haikus ===\n");
    for (int i = 0; i < 3; i++) {
        char* invalid_haiku = generate_invalid_haiku();
        bool is_valid = verify_haiku(invalid_haiku);
        print_verification_result(invalid_haiku, is_valid);
        printf("(Expected: INVALID)\n\n");
        free(invalid_haiku);
    }
    
    printf("\n=== Manual Testing ===\n");
    const char* manual_haikus[] = {
        "silent waterfall\n"
        "whispering in the mountain\n"
        "nature's melody\n\n",
        
        "autumn moonlight gleams\n"
        "shadows dancing on the lake\n"
        "peaceful night descends\n\n",
        
        "cherry blossoms fall\n"
        "pink petals drift through spring air\n"
        "beauty in decay\n\n"
    };
    
    for (int i = 0; i < 3; i++) {
        bool is_valid = verify_haiku(manual_haikus[i]);
        print_verification_result(manual_haikus[i], is_valid);
    }
    
    printf("\n=== Interactive Mode ===\n");
    printf("Enter your haiku (3 lines, press Enter twice to finish):\n");
    
    char input[1000] = {0};
    char line[100];
    int line_count = 0;
    
    while (line_count < 3) {
        if (!fgets(line, sizeof(line), stdin)) break;
        
        if (line[0] == '\n') break;
        
        strcat(input, line);
        line_count++;
    }
    
    if (strlen(input) > 0 && input[strlen(input)-1] != '\n') {
        strcat(input, "\n");
    }
    strcat(input, "\n");
    
    bool user_haiku_valid = verify_haiku(input);
    print_verification_result(input, user_haiku_valid);
    
    return 0;
}

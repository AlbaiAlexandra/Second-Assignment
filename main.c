#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_screen() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar(); // Catch the newline
}

// --- NEW: Morse Code Dictionary ---
char morse_to_char(const char* morse) {
    const char* letters[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
        "..-", "...-", ".--", "-..-", "-.--", "--.."
    };
    
    // Check A-Z
    for (int i = 0; i < 26; i++) {
        if (strcmp(morse, letters[i]) == 0) {
            return 'A' + i;
        }
    }
    return '?'; // Return '?' if it's an invalid morse sequence
}

// --- NEW: Read File and Translate Logic ---
void read_diary_file() {
    FILE *file = fopen("diary.txt", "r");
    if (file == NULL) {
        printf("\n[Error] Could not open 'diary.txt'. Make sure the file exists!\n");
        return;
    }

    printf("\n--- DIARY ENTRIES ---\n\n");

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char buffer[32] = "";
        int buf_idx = 0;
        int space_count = 0;

        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '.' || line[i] == '-') {
                // If we've seen 3 or more spaces, the previous letter is finished
                if (space_count >= 3 && buf_idx > 0) {
                    buffer[buf_idx] = '\0';
                    printf("%c", morse_to_char(buffer));
                    buf_idx = 0; // Reset buffer for the new letter

                    // If we've seen 7 or more spaces, it's also a new word
                    if (space_count >= 7) {
                        printf(" "); 
                    }
                }
                
                // Add the current dot/dash to the buffer
                buffer[buf_idx++] = line[i];
                space_count = 0; // Reset space count since we found a character
                
            } else if (line[i] == ' ') {
                space_count++;
            } else if (line[i] == '\n' || line[i] == '\r') {
                // End of line reached, translate whatever is left in the buffer
                if (buf_idx > 0) {
                    buffer[buf_idx] = '\0';
                    printf("%c", morse_to_char(buffer));
                    buf_idx = 0;
                }
                printf("\n");
                space_count = 0;
            }
        }
        
        // Catch any remaining characters if the file doesn't end with a newline
        if (buf_idx > 0) {
            buffer[buf_idx] = '\0';
            printf("%c", morse_to_char(buffer));
        }
    }

    printf("\n\n---------------------\n");
    fclose(file);
}

void diary_settings_submenu() {
    int sub_choice = -1;

    while (sub_choice != 0) {
        clear_screen();
        printf("=== DIARY SETTINGS ===\n");
        printf("1. Clear Entire Diary\n");
        printf("2. Backup Diary File\n");
        printf("0. Return to Main Menu\n");
        printf("----------------------\n");
        printf("Select an option: ");

        if (scanf("%d", &sub_choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (sub_choice == 1) {
            printf("\n[System] Diary wiped clean.");
            pause_screen();
        } else if (sub_choice == 2) {
            printf("\n[System] Diary backed up successfully.");
            pause_screen();
        }
    }
}

void print_main_menu() {
    printf("=================================\n");
    printf("    MORSE CODE DIARY APP   \n");
    printf("=================================\n");
    printf("1. Write New Entry (Saves as Morse)\n");
    printf("2. Read Diary (Translates to Text)\n");
    printf("3. Search Diary for a Word\n");
    printf("4. Diary Settings (Submenu)\n");
    printf("0. Exit Application\n");
    printf("---------------------------------\n");
}

int main() {
    int choice = -1;

    while (choice != 0) {
        clear_screen();
        print_main_menu();
        printf("Select an option: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        // Consume the newline character left in the buffer by scanf
        // This prevents issues with subsequent getchar() calls
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (choice) {
            case 1:
                printf("\n[Action] Type your text. It will be saved with 1 space between components, 3 between letters, and 7 between words.");
                pause_screen();
                break;
            case 2:
                // --- MODIFIED: Calling our new read function ---
                read_diary_file();
                pause_screen();
                break;
            case 3:
                printf("\n[Action] Enter the word you want to search for: ");
                pause_screen();
                break;
            case 4:
                diary_settings_submenu();
                break;
            case 0:
                printf("\nClosing Diary. Goodbye!\n");
                break;
            default:
                printf("\nInvalid selection. Try again.");
                pause_screen();
                break;
        }
    }

    return 0;
}
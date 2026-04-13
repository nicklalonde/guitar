// March 8 2026
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define FRETS 13 // num frets. open (0) to 12th fret
#define STRINGS 6 // num strings
#define NOTES 12 // 12 notes in the musical scale .

// Scale Masks          // Acts as a mask to determine if a note is in the scale or not.
#define MAJOR_SCALE      0xAB5 //  101010110101 (Root, 2, 3, 4, 5, 6, 7) WWHWWWH
#define NATURAL_MINOR    0x5AD //  010110101101 (Root, 2, b3, 4, 5, b6, b7)
#define PENTATONIC_MAJOR 0x295 //  001010010101 (Root, 2, 3, 5, 6)
#define PENTATONIC_MINOR 0x4A9 //  010010101001 (Root, b3, 4, 5, b7)
#define BLUES_SCALE      0x4E9 //  010011101001 (Root, b3, 4, b5, 5, b7)
#define HARMONIC_MINOR   0x9AD //  100110101101 (Root, 2, b3, 4, 5, b6, 7)
// Integer Representation of Notes:
// 0 1  2 3  4 5 6  7 8  9 10 11
// C C# D D# E F F# G G# A A# B 
const char *notes[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

// standard guitar tuning E A D G B E 
// Integer Representation of String Notes:
// (1st string) E = 4, (2nd string) B = 11, (3rd string) G = 7, (4th string) D = 2, (5th string) A = 9, (6th string) E = 4.
int tuning[STRINGS] = {4, 11, 7, 2, 9, 4};

struct Guitar { // Maybe will need more attributes later? 
    int *data; // flat array to store all notes in order with efficiency.
};

struct Guitar *create_guitar() { // creates the guitar objects
    struct Guitar *guitar = malloc(sizeof(struct Guitar));
    if(guitar == NULL) {
        printf("malloc failed.\n");
        return NULL;
    }
    guitar->data = malloc(sizeof(int) * (STRINGS * FRETS));
    
    return guitar;
}

void load_notes(struct Guitar *guitar) { // loads note integer values into array.
    for (int i = 0; i < STRINGS; i++) {
        for (int j = 0; j < FRETS; j++) {
            int index = (i * FRETS) + j; // calculates the 1D index
            guitar->data[index] = (tuning[i] + j) % 12; // creates a 1D array of all notes as int vals.
        }
    }
}

void print_notes(struct Guitar *guitar) { // converts all the note integer values into string values.
    for (int i = 0; i < STRINGS; i++) {
        for (int j = 0; j < FRETS; j++) {
            printf("%s  ", notes[guitar->data[(i * FRETS) + j]]);
        }
        printf("\n");
    }
}
int note_to_int(char *input) { // converts the string note into the proper integer.
    if (islower(input[0])) {
        input[0] = toupper(input[0]);
    }

    for (int i = 0; i < NOTES; i ++) {
        if (strcmp(input, notes[i]) == 0) {
            return i;
        }
    }
    return -1;
}
int get_root() { 
    int valid = 0;
    char input_buffer[20]; 
    int note_val;
    
    while(!valid) {
        printf("\nEnter a root note ('C', 'C#', 'D', . . .) ('q' to quit)> ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
            
            input_buffer[strcspn(input_buffer, "\n")] = 0;

            if (input_buffer[0] == 'q' || input_buffer[0] == 'Q') { // user quits.
                return -1; 
            }

            note_val = note_to_int(input_buffer);
            
            if(note_val == -1) {
                printf("Invalid input! Try again.\n");
                continue; // skips the rest of the loop and prompts them again
            }

            valid = 1; // input was good.
        }
        else {
            printf("Error reading input.\n");
        }
    }
    return note_val;
}

void scale_selection() {
    printf("Select a scale to show on the fretboard.\n");
    printf("1. Major\n");
    printf("2. Natural Minor\n");
    printf("3. Major Pentatonic\n");
    printf("4. Minor Pentatonic\n");
    printf("5. Blues\n");
    printf("6. Harmonic Minor\n");
    printf("scale> ");
    // add more later.
}
int is_note_in_scale(int note, int root, int scale_mask) { // determines if note is in scale. 

    int interval = (note - root + NOTES) % NOTES; // calculates how far the current note is from the root note.

    return (scale_mask & (1 << interval)); 
    // left shift 1 by the interval amount i.e. 
    // 1 << 4 = ...010000
    // AND this with scale_mask
    //i.e 101010110101
    //  & 000000010000
    // -> 000000010000
    // returns true if the result is > 0, (True in this case) and this note is in the scale, else returns false and note is not.
} 

void populate_scale_notes(int root, int mask, int *scale_notes) {
    int index = 0;
    
    // test every note from 0 to 11. If it's in the scale, add it to our array.
    for (int i = 0; i < NOTES; i++) {
        if (is_note_in_scale(i, root, mask)) {
            scale_notes[index++] = i; 
        }
    }
    
    if (index < NOTES) {
        scale_notes[index] = -1; // end of array val
    }
}

int get_scale(char *scale) { // returns the mask for the scale.
    int valid = 0;
    char user_scale[2];
    int mask = -1;
    char input_buffer[100];
    while (!valid) {
        scale_selection();
        if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
            if (sscanf(input_buffer, "%d", &mask) == 1) {
                switch(mask) {
                    case 1:
                        mask = MAJOR_SCALE;
                        strcpy(scale, "Major Scale");
                        valid = 1;
                        break;
                    case 2:
                        mask = NATURAL_MINOR;
                        strcpy(scale, "Natural Minor Scale");
                        valid = 1;
                        break;
                    case 3:
                        mask = PENTATONIC_MAJOR;
                        strcpy(scale, "Pentatonic Major Scale");
                        valid = 1;
                        break;
                    case 4:
                        mask = PENTATONIC_MINOR;
                        strcpy(scale, "Pentatonic Minor Scale");
                        valid = 1;
                        break;
                    case 5:
                        mask = BLUES_SCALE;
                        strcpy(scale, "Blues Scale");
                        valid = 1;
                        break;
                    case 6:
                        mask = HARMONIC_MINOR;
                        strcpy(scale, "Harmonic Minor Scale");
                        valid = 1;
                        break;
                    default:
                        printf("Invalid selection. Please enter a number between 1 and 6.\n");
                }
            } else {
                printf("Invalid input. Please enter a number.\n");
            }
        }
    }
    return mask;
}

void display_scale_fretboard(struct Guitar *g, int root, int mask, char *scale, int *scale_notes) {
    int index = 0;
    printf("Fretboard for the %s %s:\n\n", notes[root], scale);
    for (int i = 0; i < FRETS; i++) {
        printf(" %-2d |", i);
    }
    printf("\n\n");
    // iterate through every note in the g->data array.
    for (int s = 0; s < STRINGS; s++) {
        for (int f = 0; f < FRETS; f++) {
            int note = g->data[(s * FRETS) + f];
            if (is_note_in_scale(note, root, mask)) { // using the masks, checks if the note is in the scale. 
                printf(" %-2s |", notes[note]);
            } else {
                printf("----|");
            }
        }
        printf("\n\n"); 
    }
}
void bubble_sort(int *scale_notes, int num_notes) {
    if (scale_notes == NULL || num_notes <= 0) return;

    for (int i = 0; i < num_notes - 1; i++) {
        for (int j = 0; j < num_notes - i - 1; j++) {
            if (scale_notes[j] > scale_notes[j + 1]) {
                // Swap the adjacent notes
                int temp = scale_notes[j];
                scale_notes[j] = scale_notes[j + 1];
                scale_notes[j + 1] = temp;
            }
        }
    }
    // Print the sorted scale
    for (int i = 0; i < num_notes; i++) {
        printf("%d ", scale_notes[i]);
    }
    printf("\n");
}



void print_scale_notes(int root, int mask) {
    printf("Notes: ");

    // check all 12 chromatic intervals starting from the root
    for (int i = 0; i < NOTES; i++) {
        
        // Start at root, add the current interval step, and wrap around 12
        int current_note = (root + i) % NOTES;

        // use bitmask logic to see if this note belongs in the scale
        if (is_note_in_scale(current_note, root, mask)) {
            printf("%s ", notes[current_note]);
        }
    }
    printf("\n");
}
int main() {
    int curr_root;
    char *scale = malloc(sizeof(char) * 100);
    int *scale_notes = malloc(sizeof(int) * NOTES);
    struct Guitar *guitar = create_guitar();
    if(guitar == NULL) return 1;
    load_notes(guitar);
    printf("Welcome to my guitar application.\n");
    while(1) {
        int root = get_root();
        if (root == -1) break;
        int mask = get_scale(scale);
        populate_scale_notes(root, mask, scale_notes);
        display_scale_fretboard(guitar, root, mask, scale, scale_notes);
        print_scale_notes(root, mask);
    }
    free(guitar->data);
    free(guitar);
    free(scale);
    free(scale_notes);
    return 0;
}
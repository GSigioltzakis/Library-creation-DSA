#include "Library.h"

int SLOTS;
library_t library;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Library.h" // You must have this header file

#define MAX_LINE 1024 // Max buffer size for a line


void library_init(void);      // Optional: Initialize main library struct
void library_destroy(void);   // Free all allocated memory
void set_total_slots(int slots);
// void register_genre(int gid, char *name);
// void register_book(int bid, int gid, char *title);
// void register_member(int sid, char *name);
// void loan_book(int sid, int bid);
// void return_book(int sid, int bid, char *score_str, char *status);
// void distribute_display(void);
// void print_genre(int gid);
// void print_member(int sid);
// void print_display(void);
// void print_stats(void);

/*we need to make a function that doesnt end in a string with 2 words but taking both those words as an input later*/
char* trim_and_dequote(char *str) {
    char *start = str;
    char *end;

    // Trim leading whitespace
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // Trim trailing whitespace
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    // Handle quotes
    if (*start == '"' && *end == '"') {
        start++; // Skip leading quote
        *end = '\0'; // Remove trailing quote
    }
    
    return start;
}


int main(int argc, char *argv[]) {
    
    FILE *event_file;
    char line_buffer[MAX_LINE];
    char *trimmed_line;
    char command[10]; // To store the command token (e.g., "BK", "PG")

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    event_file = fopen(argv[1], "r");
    if (!event_file) {
        perror("fopen error for event file open");
        exit(EXIT_FAILURE);
    }

    // library_init(); // Call your initialization function here

    while (fgets(line_buffer, MAX_LINE, event_file)) {
        
        // First, trim any whitespace leading the line
        trimmed_line = line_buffer;
        while (trimmed_line && isspace(*trimmed_line)) {
            trimmed_line++;
        }

        if (!trimmed_line || *trimmed_line == '\0' || *trimmed_line == '#') {
            continue;
        }

        // Parse the first word as the command token
        if (sscanf(trimmed_line, "%9s", command) != 1) {
            continue; 
        }

        if (strcmp(command, "S") == 0) { 
            int slots;
            if (sscanf(trimmed_line, "S %d", &slots) != 1) {
                fprintf(stderr, "Event S parsing error\n");
            } else {
                set_total_slots(slots);
            }
        } 
        // else if (strcmp(command, "G") == 0) {
        //     int gid;
        //     char name_buffer[MAX_LINE];
        //     if (sscanf(trimmed_line, "G %d %[^\n]", &gid, name_buffer) != 2) {
        //         fprintf(stderr, "Event G parsing error\n");
        //     } else {
        //         register_genre(gid, trim_and_dequote(name_buffer));
        //     }
        // } 
        // else if (strcmp(command, "BK") == 0) { 
        //     int bid, gid;
        //     char title_buffer[MAX_LINE];
        //     if (sscanf(trimmed_line, "BK %d %d %[^\n]", &bid, &gid, title_buffer) != 3) {
        //         fprintf(stderr, "Event BK parsing error\n");
        //     } else {
        //         register_book(bid, gid, trim_and_dequote(title_buffer));
        //     }
        // } 
        // else if (strcmp(command, "M") == 0) {
        //     int sid;
        //     char name_buffer[MAX_LINE];
        //     if (sscanf(trimmed_line, "M %d %[^\n]", &sid, name_buffer) != 2) {
        //         fprintf(stderr, "Event M parsing error\n");
        //     } else {
        //         register_member(sid, trim_and_dequote(name_buffer));
        //     }
        // } 
        // else if (strcmp(command, "L") == 0) { 
        //     int sid, bid;
        //     if (sscanf(trimmed_line, "L %d %d", &sid, &bid) != 2) {
        //         fprintf(stderr, "Event L parsing error\n");
        //     } else {
        //         loan_book(sid, bid);
        //     }
        // } 
        // else if (strcmp(command, "R") == 0) {
        //     int sid, bid;
        //     char score_str[10], status_str[10];
        //     if (sscanf(trimmed_line, "R %d %d %9s %9s", &sid, &bid, score_str, status_str) != 4) {
        //         fprintf(stderr, "Event R parsing error\n");
        //     } else {
        //         return_book(sid, bid, score_str, status_str);
        //     }
        // } 
        // else if (strcmp(command, "D") == 0) { 
        //     distribute_display();
        // } 
        // else if (strcmp(command, "PG") == 0) { 
        //     int gid;
        //     if (sscanf(trimmed_line, "PG %d", &gid) != 1) {
        //         fprintf(stderr, "Event PG parsing error\n");
        //     } else {
        //         print_genre(gid);
        //     }
        // } 
        // else if (strcmp(command, "PM") == 0) { 
        //     int sid;
        //     if (sscanf(trimmed_line, "PM %d", &sid) != 1) {
        //         fprintf(stderr, "Event PM parsing error\n");
        //     } else {
        //         print_member(sid);
        //     }
        // } 
        // else if (strcmp(command, "PD") == 0) { 
        //     print_display();
        // } 
        // else if (strcmp(command, "PS") == 0) { 
        //     print_stats();
        // } 
        else {
            fprintf(stderr, "WARNING: Unrecognized event %s. Continuing...\n", command);
        }
    }
    fclose(event_file);
    //library_destroy();
    
    return 0;
}
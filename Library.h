#ifndef LIBRARY_H
#define LIBRARY_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define TITLE_MAX 128
#define NAME_MAX  64

/*  Παγκόσμια θέση προβολών (ορίζεται από την εντολή S <slots>)
    Φροντίστε να το κάνετε define στο αντίστοιχο .c αρχείο, ως
    int SLOTS;
*/
extern int SLOTS;
extern struct library *Library;

/* -----------------------------------------
   LOAN: ενεργός δανεισμός (unsorted, O(1) insert/remove)
   Λίστα ανά Member με χρήση sentinel node.
   ----------------------------------------- */
typedef struct loan {
    int sid;            /* member id (ιδιοκτήτης της λίστας) */
    int bid;            /* book id που έχει δανειστεί */
    struct loan *next;  /* επόμενος δανεισμός του μέλους */
} loan_t;

/* -----------------------------------------
   BOOK: βιβλίο
   - Ανήκει σε ακριβώς ένα Genre (gid)
   - Συμμετέχει στη διπλά συνδεδεμένη λίστα του Genre,
     ταξινομημένη φθίνοντα κατά avg.
   ----------------------------------------- */
typedef struct book {
    int  bid;                         /* book id (μοναδικό) */
    int  gid;                         /* genre id (ιδιοκτησία λίστας) */
    char title[TITLE_MAX];

    /* Στατιστικά δημοτικότητας */
    int sum_scores;                   /* άθροισμα έγκυρων βαθμολογιών */
    int n_reviews;                    /* πλήθος έγκυρων βαθμολογιών */
    int avg;                          /* cache: floor(sum_scores / n_reviews); 0 αν n_reviews=0 */
    int lost_flag;                    /* 1 αν δηλωμένο lost, αλλιώς 0 */

    /* Διπλά συνδεδεμένη λίστα του genre, ταξινομημένη κατά avg (desc). */
    struct book *prev;
    struct book *next;

    /* Προαιρετικό: συνδέσεις σε global ευρετήρια αν κρατήσετε (όχι απαραίτητο) */
    // struct book *next_global;         /* π.χ. unsorted λίστα όλων των βιβλίων */
} book_t;

/* -----------------------------------------
   MEMBER: μέλος βιβλιοθήκης
   - Κρατά unsorted λίστα ενεργών δανεισμών (loan_t) με χρήση sentinel node
   ----------------------------------------- */
typedef struct member {
    int  sid;                         /* member id (μοναδικό) */
    char name[NAME_MAX];

    /* Λίστα ενεργών δανεισμών:
       Uns. singly-linked με sentinel node:
       - Εισαγωγή: O(1) push-front
       - Διαγραφή γνωστού bid: O(1) αν κρατάτε prev pointer στη σάρωση */
    loan_t* loans;

    /* Μονοσυνδεδεμένη λίστα όλων των μελών ταξινομημένη κατά sid */
    struct member *next;
} member_t;

/* -----------------------------------------
   GENRE: κατηγορία βιβλίων
   - Κρατά ΔΙΠΛΑ συνδεδεμένη λίστα ΒΙΒΛΙΩΝ ταξινομημένη κατά avg (desc)
   - Κρατά και το αποτέλεσμα της τελευταίας D (display) για εκτύπωση PD
   ----------------------------------------- */
typedef struct genre {
    int  gid;                         /* genre id (μοναδικό) */
    char name[NAME_MAX];

    /* διπλά συνδεδεμένη λίστα βιβλίων ταξινομημένη κατά avg φθίνουσα. */
    book_t* books;
    int lost_count;
    int invalid_count;

    /* Αποτέλεσμα τελευταίας κατανομής D: επιλεγμένα βιβλία για προβολή.
       Αποθηκεύουμε απλώς pointers στα book_t (δεν αντιγράφουμε δεδομένα). */
    int slots;            /* πόσα επιλέχθηκαν για προβολή σε αυτό το genre */
    book_t **display;     /* δυναμικός πίνακας με pointers στα επιλεγμένα βιβλία για προβολή */

    /* Μονοσυνδεδεμένη λίστα όλων των genres ταξινομημένη κατά gid (για εύκολη σάρωση). */

   /*NEW FIELDS FOR EASIER APPROACH AT EVENT D(DISPLAY)*/
      int calc_points;      /* υπολογισμένοι πόντοι για κατανομή θέσεων */
      int calc_rem;         /* υπολογισμένο υπόλοιπο για κατανομή θέσεων */   
      

    struct genre *next;
} genre_t;

/* -----------------------------------------
   LIBRARY: κεντρικός "ρίζας"
   - Κρατά λίστα Genres (sorted by gid)
   - Κρατά λίστα Members (sorted by sid)
   ----------------------------------------- */
typedef struct library {
    genre_t  *genres;     /* κεφαλή λίστας genres (sorted by gid) */
    member_t *members;    /* διπλά συνδεδεμένη λίστα μελών (sorted by sid) */
    // book_t   *books;      /* unsorted λίστα όλων των books (ευκολία αναζήτησης) — προαιρετικό */
} library_t;

/* =========================
   ΒΟΗΘΗΤΙΚΕΣ ΣΥΜΒΑΣΕΙΣ & INVARIANTS
   =========================

   ΕΙΣΑΓΩΓΗ βιβλίου σε genre (μετά από αλλαγή avg):
   - Επανατοποθέτηση με τοπικές αλλαγές δεικτών (O(hops)):
     1) Αφαιρείς το βιβλίο από την τωρινή θέση (splice-out).
     2) Σαρώνεις προς τα πάνω/κάτω μέχρι να βρεις τοπική θέση ως προς avg.
     3) Εισάγεις (splice-in) πριν από τον πρώτο κόμβο με avg < δικό μου.

   DISPLAY (αποτέλεσμα D):
   - Πριν τρέξει νέα D, απελευθερώνεις την προηγούμενη μνήμη (free genre->display),
     και θέτεις display=NULL, display_count=0 σε όλα τα genres.
   - Η D ΓΕΜΙΖΕΙ τον πίνακα display ανά genre με έως seats(g) pointers.

   ΠΟΛΥΠΛΟΚΟΤΗΤΕΣ:
   - Insert loan: O(1)
   - Return book με έγκυρο score: ενημέρωση sum/n/avg (O(1)) + επανατοποθέτηση σε genre (O(hops))
   - Υπολογισμός D: μία σάρωση όλης της λίστας κάθε genre για points (O(#books_in_genre))
     + ταξινόμηση remainders (O(#genres log #genres))
     + επιλογή κορυφαίων ανά genre (γραμμική στα seats(g)).
*/
char* trim_and_dequote(char *str);
void library_init(void);      // Optional: Initialize main library struct
void Slots(int slots);
void Genre_book(int gid, char *name);
void register_book(int bid, int gid, char *title);
void register_member(int sid, char *name);
void loan_book(int sid, int bid);
void return_book(int sid, int bid, char *score_str, char *status);
void display(void);
// void print_genre(int gid);
// void print_member(int sid);
// void print_display(void);
// void print_stats(void);
// void library_free(void);  // Optional: Free all allocated memory





#endif

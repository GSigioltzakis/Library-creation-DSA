#include "Library.h"


/*we have an empty library at first*/
void library_init(void) {
    Library = (library_t *)malloc(sizeof(library_t));
    Library->genres = NULL;
    Library->members = NULL;
}

void Slots(int slots) {
    SLOTS = slots;
    printf("Done\n");
}

void Genre_book(int gid, char *name) {
    /*we need to visit the list and see if we can find the book based on its id and name, therefore we need to set the whole
    genre struct to NULL,0 or allocate it to use.*/

    genre_t *curr = Library->genres;
    genre_t *prev = NULL;
    while (curr != NULL && curr->gid < gid) {
        prev = curr;
        curr = curr->next;
    }//small traversal to find the right genre

    /*we must ignore the same book*/
    if (curr != NULL && curr->gid == gid) {
        printf("IGNORED\n");
        return;
    }

    /*we create the new genre*/
    genre_t *new_genre = (genre_t *)malloc(sizeof(genre_t));
    if(new_genre == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    /*we set the struct genre:*/
    new_genre->gid = gid;
    int i;
    for (i = 0; i < (NAME_MAX - 1) && name[i] != '\0'; i++) {
       new_genre->name[i] = name[i];
    }
    new_genre->name[i] = '\0'; /*null termination*/
    
    new_genre->books = NULL;
    new_genre->lost_count = 0;
    new_genre->invalid_count = 0;
    new_genre->slots = 0;
    new_genre->display = NULL;

    /*we insert the new genre in the right place*/
    new_genre->next = curr;
    if (prev == NULL) {
        Library->genres = new_genre;
    } else {
        prev->next = new_genre; //link previous to new
    }

    printf("DONE\n");

}

void register_book(int bid, int gid, char *title) { //supposly BK means book
    
    
    /*find the genre first*/
    genre_t *curr_genre = Library->genres;
    genre_t *target = NULL;
    while (curr_genre != NULL) {
        if (curr_genre->gid == gid) {
            target = curr_genre;
            break;
        }
        curr_genre = curr_genre->next;
    }
    if (target == NULL) {
        printf("IGNORED\n");
        return;
    }
    /*check if the book already exists*/
    curr_genre = Library->genres;
    while (curr_genre != NULL) {
        book_t *curr_book = curr_genre->books;
        while (curr_book != NULL) {
            if (curr_book->bid == bid) {
                printf("IGNORED\n");
                return;
            }
            curr_book = curr_book->next;
        }
        curr_genre = curr_genre->next;
    } //check all genres for existing book /end/

    /*Now we cheched all conditions for a book existing already and finding the book's genre, so now we can
    create a new book(and instert it)*/

    book_t *new_book = (book_t *)malloc(sizeof(book_t));
    new_book->bid = bid;
    new_book->gid = gid;
    int i; //we set it outside the for loop to use it after    
    for (i = 0; i < (TITLE_MAX - 1) && title[i] != '\0'; i++) {
        new_book->title[i] = title[i];
    } //putting the title in the struct 
    new_book->title[i] = '\0'; //ending with null termination
        
    new_book->sum_scores = 0;
    new_book->n_reviews = 0;
    new_book->avg = 0;
    new_book->lost_flag = 0;
    /*the list must be sorted decending based of the avg, so we already have avg=0 meaning we start from tail (lets call 
    it the first node). */
    new_book->next = NULL;
    if(target->books == NULL){
        target->books = new_book;
        new_book->prev = NULL;
    }else{
        book_t *temp = target->books; //start from tail
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new_book;
        new_book->prev = temp;
    }
    printf("DONE\n");
}

void register_member(int sid, char *name){
    /*the register member's function, we can implement it similar with the Genre()function, only difference is we will add a
    sentinel node for easier. We will need it on later functions for O(1) insertions and deletions if needed.*/


    member_t *curr = Library->members;
    member_t *prev = NULL;
    while (curr != NULL && curr->sid < sid) {
        prev = curr;
        curr = curr->next;
    }
    if(curr != NULL && curr->sid == sid){
        printf("IGNORED\n");
        return;
    }
    member_t *new_member = (member_t *)malloc(sizeof(member_t));
    if(new_member == NULL){
        printf("Memory allocation failed\n");
        return;
    }
    new_member->sid = sid;
    int i;
    for (i = 0; i < (NAME_MAX - 1) && name[i] != '\0'; i++) {
        new_member->name[i] = name[i];
    }
    new_member->name[i] = '\0'; /*null termination*/
    /*----SENTINEL NODE CREATION----*/
    loan_t *sentinel = (loan_t *)malloc(sizeof(loan_t));
    if(sentinel == NULL){
        printf("Memory allocation failed\n");
        free(new_member); //free previously allocated memory
        return;
    }
    sentinel->sid = -1; /*with the -1 we understand which 'node' lets say, is the sentinel one */
    sentinel->bid = -1; 
    sentinel->next = NULL;
    new_member->loans = sentinel;

    /*we insert the new member in the right place*/
    new_member->next = curr;
    if (prev == NULL) {
        Library->members = new_member;
    } else {
        prev->next = new_member; //link previous to new
    }
    printf("DONE\n");
}
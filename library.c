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
    strncpy(new_genre->name, name, NAME_MAX-1);/*we have to set size MAX-1*/
    new_genre->name[NAME_MAX-1] = '\0'; /*null termination*/
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
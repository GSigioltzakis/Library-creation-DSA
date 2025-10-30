#include "Library.h"


/*we have an empty library at first*/
void library_init(void) {
    Library = (library_t *)malloc(sizeof(library_t));
    Library->genres = NULL;
    Library->members = NULL;
}

void Slots(int slots) {
    SLOTS = slots;
    // if (slots < 0) {  removed for event D(checked there)
    //     printf("IGNORED\n");
    //     return;
    // }
    printf("<S>     Done\n");
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

    printf("<G>     DONE\n");

}

void register_book(int bid, int gid, char *title) { //supposly BK means book
    /*find the genre first*/
    genre_t *curr_genreenre = Library->genres;
    genre_t *target = NULL;
    while (curr_genreenre != NULL) {
        if (curr_genreenre->gid == gid) {
            target = curr_genreenre;
            break;
        }
        curr_genreenre = curr_genreenre->next;
    }
    if (target == NULL) {
        printf("IGNORED\n");
        return;
    }
    /*check if the book already exists*/
    curr_genreenre = Library->genres;
    while (curr_genreenre != NULL) {
        book_t *curr_bookook = curr_genreenre->books;
        while (curr_bookook != NULL) {
            if (curr_bookook->bid == bid) {
                printf("IGNORED\n");
                return;
            }
            curr_bookook = curr_bookook->next;
        }
        curr_genreenre = curr_genreenre->next;
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
    printf("<BK>    DONE\n");
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
    printf("<M>     DONE\n");
}

/*we will play with the sentinel node in this list. We wil add him at the START lets say, to achive O(1) search, or if we put 
him at the end we will achive O(n), something that we dont want. */
/*What we want: find the id of the book, then the sid, and then find a way to INSERT (loan) a book*/
void loan_book(int sid, int bid) {
    genre_t *curr_genre = Library->genres; /*FINDING BID*/
    book_t *target_book = NULL; 
    
    while (curr_genre != NULL) {
        book_t *curr_book = curr_genre->books;
        while (curr_book != NULL) {
            if (curr_book->bid == bid) {
                target_book = curr_book;//FOUND!
                break;
            }
            curr_book = curr_book->next;
        }
        if (target_book != NULL) {
            break; //we found it so we break 
        }
        curr_genre = curr_genre->next;
    }

    if (target_book == NULL) {
        printf("IGNORED\n"); 
        return;
    }
    /*sid*/
    member_t *curr_member = Library->members;
    member_t *target_member = NULL;
    while (curr_member != NULL) {
        if (curr_member->sid == sid) {
            target_member = curr_member; //FOUND!
            break;
        }
        curr_member = curr_member->next;
    }
    if (target_member == NULL) {
        printf("IGNORED\n");
        return;
    }

    /*Now we have both the book and the member, we need to check if the member has already loaned this book*/
    loan_t *curr_loan = target_member->loans->next; //start from the first real loan (after sentinel)
    while (curr_loan != NULL) {
        if (curr_loan->bid == bid) {
            printf("IGNORED\n"); //already loaned
            return;
        }
        curr_loan = curr_loan->next;
    }
    /*Now we can create the new loan and insert it at the start (after sentinel)*/
    loan_t *new_loan = (loan_t *)malloc(sizeof(loan_t));
    if (new_loan == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    new_loan->sid = sid;
    new_loan->bid = bid;
    /*instertion: O(1) push-front after sentinel, so we need to do it in a way we reference it to the member. Thats why we
    are going to set the new loan to the member's-next-loan.  */
    new_loan->next = target_member->loans->next;
    target_member->loans->next = new_loan;
    printf("<L>     DONE\n");
}
/* @NOTE: in the function's parameters, we set the score variable to a string due to "NA" string in 
test files. Therefore, i had to use a function that turns a string to int(meaning the string "11"->gives us
int 11), and that function is atoi(); */
void return_book(int sid, int bid, char *score_str, char *status){
    /*now we need to return a book, meaning we must "delete/remove" the book that a member loaned
    and remove it from the loan list,.*/
    /*before that, we have to find the member in the library members, then find the book bid
    and then store the genre id (gid)*/
    member_t *curr_member = Library->members;
    member_t *target_member = NULL;
    while (curr_member != NULL) {
        if (curr_member->sid == sid) {
            target_member = curr_member; //FOUND!
            break;
        }
        curr_member = curr_member->next;
    }
    if (target_member == NULL) {
        printf("IGNORED\n");
        return;
    }
    /*finding the bid among the sid loans*/
    loan_t *prev_loan = target_member->loans; //start from sentinel
    loan_t *curr_loan = target_member->loans->next;
    loan_t *target_loan = NULL;
    while (curr_loan != NULL) {
        if (curr_loan->bid == bid) {
            target_loan = curr_loan; //FOUND!
            break;
        }
        prev_loan = curr_loan;
        curr_loan = curr_loan->next;
    }
    if (target_loan == NULL) {
        printf("IGNORED\n"); //the member didnt loan this book
        return;
    }
    /*and now the bid*/
    genre_t *curr_genre = Library->genres; /*FINDING BID*/
    book_t *target_book = NULL;
    while (curr_genre != NULL) {
        book_t *curr_book = curr_genre->books;
        while (curr_book != NULL) {
            if (curr_book->bid == bid) {
                target_book = curr_book;//FOUND!
                break;
            }
            curr_book = curr_book->next;
        }
        if (target_book != NULL) {
            break; //we found it so we break 
        }
        curr_genre = curr_genre->next;
    }
    if (target_book == NULL) {
        printf("IGNORED\n"); 
        return;
    }
    /*Now we have both the book and the member, we can proceed to the return process*/
    /*first we remove the loan from the member's loan list*/
    genre_t *book_genre = curr_genre; //we keep the genre of the book for later use
    
    prev_loan->next = target_loan->next;
    free(target_loan); //free the memory allocated for the loan
    /*now we handle the book return based on status*/
    if (strcmp(status, "lost") == 0) {
        target_book->lost_flag = 1;
        book_genre->lost_count += 1;
        printf("<R>     DONE\n");
    } else if(strcmp(status, "ok")==0){
        if(strcmp(score_str, "NA")==0){
            printf("<R>     DONE\n");
        }else{
            int score = atoi(score_str); //convert string to int (due to "NA")
            if (score < 0 || score > 10) {//invalid
                book_genre->invalid_count++;
                printf("IGNORED\n");
            
            } else {
                target_book->sum_scores += score;
                target_book->n_reviews += 1;
                target_book->avg = target_book->sum_scores / target_book->n_reviews;

                if (target_book->prev != NULL) {
                    target_book->prev->next = target_book->next;
                } else {
                    book_genre->books = target_book->next; //the head of the list that we spliced out(removed)
                }
                
                if (target_book->next != NULL) { 
                    target_book->next->prev = target_book->prev; 
                }
                
                //splice-in
                book_t *insertion_point = book_genre->books;
                book_t *prev_point = NULL;
                /*now we need to find where we inserted (loaned)the member's book*/
                while (insertion_point != NULL && insertion_point->avg > target_book->avg) { 
                    prev_point = insertion_point;
                    insertion_point = insertion_point->next;
                }
                //lets assume there is an equal avg, we must sort based on bid ascending
                if (insertion_point != NULL && insertion_point->avg == target_book->avg) {
                    while (insertion_point != NULL && insertion_point->avg == target_book->avg && insertion_point->bid < target_book->bid) {
                        prev_point = insertion_point;
                        insertion_point = insertion_point->next;
                    }
                }

                
                target_book->next = insertion_point; 
                target_book->prev = prev_point;
                
                if (prev_point != NULL) {
                    prev_point->next = target_book;
                } else {
                    book_genre->books = target_book; /*our new head, after we checked and set next and prev*/
                }

                if (insertion_point != NULL) {
                    insertion_point->prev = target_book; 
                }

                //printf("DONE\n");
            }
        }
    } else {
        printf("IGNORED\n");
    }
    printf("<R>     DONE\n");
}
/*in this event we must distripute the slots based on calculations and genres.
The goal is to see the most liked book from each genre.*/
void display(void) {    
    /*How to achive this: we have 5 phases; 
        1) for each genre/category calculate the points and find their sum score with n_revies>0 and lost_flag=0
        2) we must calculate a "quota" meaning our Valid points from (1) / our SLOTS
        3) need to calculate the seats by; points/quota
        4) need to calculate the remain: points-(seats*quota) and share the remaining seats
        based on size(start frm most remain)(if tie then least remain)
        5) now we find our most liked book: for each genre; find the most seats(with lost_flag=0)
        and sort the list based on their avarage(avg). if the books are not that many then the seats 
        remain empty
    */
    if (SLOTS <= 0 || Library->genres == NULL) {
        printf("<D>     DONE\n");
        return;
    }

    genre_t *curr_genre = Library->genres;
    int total_valid_points = 0;

    /* 1) traversing the list */
    while (curr_genre != NULL) { //reset calculations
        curr_genre->calc_points = 0;
        curr_genre->calc_rem = 0;
        curr_genre->slots = 0;
        if (curr_genre->display != NULL) { 
            free(curr_genre->display);//old array 
            curr_genre->display = NULL;
        }

        book_t *curr_book = curr_genre->books; 
        while (curr_book != NULL) {
            if (curr_book->n_reviews > 0 && curr_book->lost_flag == 0) { //if the book is valid
                curr_genre->calc_points += curr_book->sum_scores; //then add its sum_scores to genre's points
            }
            curr_book = curr_book->next;
        }
        total_valid_points += curr_genre->calc_points; //points
        curr_genre = curr_genre->next;
    }

    if (total_valid_points == 0) {
        printf("<D>     DONE\n");
        return;
    }

    /* 2)*/
    int quota = floor(total_valid_points / SLOTS); //needed flooring
    int remaining_slots = SLOTS;

    /* 3) calculate seats and remainers */
    curr_genre = Library->genres;
    while (curr_genre != NULL) {
        if (quota > 0) {
            curr_genre->slots = curr_genre->calc_points / quota;
            curr_genre->calc_rem = curr_genre->calc_points % quota;
        } else {
            curr_genre->slots = 0;
            curr_genre->calc_rem = curr_genre->calc_points;
        }
        remaining_slots -= curr_genre->slots;
        curr_genre = curr_genre->next;
    }

    /*4) distributing remaining slots. We need to have a complexity of O(NlogN) so we will do a simple selection method.
    This method might be the most efficient, but we will traverse the list each time we want to find the best remain.*/

    while (remaining_slots > 0) {
        genre_t *winner = NULL; //we want to find the genre with the best remain!
        int best_rem = -1;

        curr_genre = Library->genres;
        while (curr_genre != NULL) { //traversal
            if (curr_genre->calc_rem > best_rem) {//finding te BEST remain
                best_rem = curr_genre->calc_rem;
                winner = curr_genre;
            } else if (curr_genre->calc_rem == best_rem) {
                //tie
                if (winner == NULL || curr_genre->gid < winner->gid) {
                    winner = curr_genre;
                }
            }
            curr_genre = curr_genre->next;
        }

        if (winner == NULL) break; 

        winner->slots++;
        winner->calc_rem = -1; 
        remaining_slots--; 
    }

    /*5) now we must create the display array for each genre*/
    /*since we updated the struct genre with 2 new fields, we need to (not exactly reallocate) but put new memory for the pointers*/
    curr_genre = Library->genres;
    while (curr_genre != NULL) {
        if (curr_genre->slots > 0) {
            //for the new pointer array
            curr_genre->display = (book_t **)calloc(curr_genre->slots, sizeof(book_t *));
            
            book_t *b_iter = curr_genre->books; //O(N) traversal
            int filled_count = 0;
            
            while (b_iter != NULL && filled_count < curr_genre->slots) {
                if (b_iter->lost_flag == 0) { 
                    curr_genre->display[filled_count] = b_iter;
                    filled_count++;
                }
                b_iter = b_iter->next;
            }
        }
        curr_genre = curr_genre->next;
    }

    printf("<D>     DONE\n");
}

void print_genre(int gid) {
    genre_t *curr_genre = Library->genres;
    while (curr_genre != NULL) {
        if (curr_genre->gid == gid) {
            break; //FOUND!
        }
        curr_genre = curr_genre->next;
    }
    if (curr_genre == NULL) {
        printf("IGNORED\n");
        return;
    }
    printf("Genre <%d>: <%s>\n", curr_genre->gid, curr_genre->name);
    book_t *curr_book = curr_genre->books;
    while (curr_book != NULL) {
        printf("      <%d>, <%d>\n", curr_book->bid, curr_book->avg);
        
        curr_book = curr_book->next;
    }
}
void print_member(int sid) {
    member_t *target_member = NULL;
    member_t *curr_m = Library->members;
    while (curr_m != NULL && curr_m->sid < sid) {
        curr_m = curr_m->next;
    }
    if (curr_m != NULL && curr_m->sid == sid) {//FOUND
        target_member = curr_m;
    }
    if (target_member == NULL) {
        printf("IGNORED(not found <PM>\n");
        return;
    }
    printf("Loans:\n");

    if (target_member != NULL) {
        loan_t *curr_loan = target_member->loans->next; //after sentinel
        
        while (curr_loan != NULL) {
            printf("    <%d>\n", curr_loan->bid);
            curr_loan = curr_loan->next;
        }
    }else printf("    (book returned :) )\n");
}
void print_display(void) {
    printf("Display:\n");
    
    int empty = 0;
    genre_t *g_iter = Library->genres;

    //traverse the list of genres 
    while (g_iter != NULL) {
        if (g_iter->slots > 0 && g_iter->display != NULL) {
            empty = 1;
            printf("    <%d>:\n", g_iter->gid);

            //traversing the display array
            int i;
            for (i = 0; i < g_iter->slots; i++) {
                book_t *book = g_iter->display[i];
                
                if (book != NULL) { 
                    printf("    <%d>, <%d>\n", book->bid, book->avg);
                }
            }
        }
        g_iter = g_iter->next;
    }
    if (empty == 0) {
        printf("    (empty)\n"); 
    }
}
void print_stats(void) {
    printf("SLOTS=<%d>\n", SLOTS);
    genre_t *g_iter = Library->genres;

    while (g_iter != NULL) {
        int current_points = 0;
        book_t *b_iter = g_iter->books;

        while (b_iter != NULL) {
            if (b_iter->n_reviews > 0 && b_iter->lost_flag == 0) { //from the display criteria
                current_points += b_iter->sum_scores; 
            }
            b_iter = b_iter->next;
        }

        printf("    <%d>:\n", g_iter->gid);
        printf(" points=<%d>\n", current_points);

        g_iter = g_iter->next;
    }
}
void free_books(book_t *book) {
    while (book) {
        book_t *next = book->next;
        free(book);
        book = next;
    }
}

void free_genres(genre_t *genre) {
    while (genre) {
        genre_t *next = genre->next;
        free_books(genre->books);
        free(genre->display);
        free(genre);
        genre = next;
    }
}

void free_loans(loan_t *loan) {
    while (loan) {
        loan_t *next = loan->next;
        free(loan);
        loan = next;
    }
}

void free_members(member_t *member) {
    while (member) {
        member_t *next = member->next;
        free_loans(member->loans);
        free(member);
        member = next;
    }
}

void library_free(void) {
    if (!Library) return;
    free_genres(Library->genres);
    free_members(Library->members);
    free(Library);
    Library = NULL;
}
#ifndef __ROWS_H
#define __ROWS_H

#include "utils.h"

// should print line numbers ?
#define HIDE_LN_IDX 0
#define PRINT_LN_IDX 1

/* Rows are saved as a linked list on the heap.
 * This may not be the most efficent way, but it works.
 * Each row is aware to the one below, and the one after.
 */
typedef struct row_t {
	// len should be strlen(text)+1
	int len;
	char* text;
	struct row_t* prev;
	struct row_t* next;
} Row;

struct row_t* new_row(char* text, int len, struct row_t* prev, struct row_t* next);
int free_row(struct row_t * row);
int free_all_rows(struct row_t * row);

struct row_t* traverse_to_end(struct row_t* row);
struct row_t* traverse_to_start(struct row_t* row);
int split_row(struct row_t * curr_row, int index); 
struct row_t* remove_row(struct row_t* row);

int jump_rows(struct row_t** row_p, int size);

void print_snippet(struct row_t* row, int size);
void print_all_rows(struct row_t* row);
void print_all_rows_ordered(struct row_t* row, int line_numbers);

#endif

#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "include/rows.h"

struct row_t* new_row(char* text, int len, struct row_t* prev, struct row_t* next) {
	
	Row* row = (Row*)malloc(sizeof(Row));
	if (!row) {
		return NULL;
	}
	row->text = text;
	row->len = len;
	row->prev = prev;
	row->next = next;
	return row;
}

int free_row(struct row_t * row) {
	if (row == NULL) {
		return FINE;
	}
	free(row->text);
	free(row);
	return FINE;
}

int free_all_rows(struct row_t * row) {
	if (row == NULL) {
		return FINE;
	}
	struct row_t* next = row->next;
	struct row_t* prev = row->prev;
	free_row(row);
	while (next || prev) {
		if (next) {
			struct row_t* tmp = next->next;
			free_row(next);
			next = tmp;
		}
		if (prev) {
			struct row_t* tmp = prev->prev;
			free_row(prev);
			prev = tmp;
		}
	}
	return FINE;
}

struct row_t* traverse_to_end(struct row_t* row){
	if (row) {
		while (row->next) {
			row = row->next;
		}
	}
	return row;
}

struct row_t* traverse_to_start(struct row_t* row){
	if (row) {
		while (row->prev) {
			row = row->prev;
		}
	}
	return row;
}

int split_row(struct row_t * curr_row, int index) {
	// TODO:: fix when spliting a row with 0
	if (index > curr_row->len) {
		return ERR;
	}
	if (index < 0) {
		index = curr_row->len / 2;
	}
	Row* new_row = (Row*)malloc(sizeof(Row));
	if (!new_row) {
		return ERR;
	}
	new_row->len = curr_row->len - index;
	new_row->text = (char*)malloc(sizeof(char)*new_row->len);
	if (!new_row->text) {
		free(new_row);
		return ERR;
	}
	memcpy(new_row->text, curr_row->text+index, new_row->len);
	curr_row->text = (char*)realloc(curr_row->text, index+1);
	curr_row->text[index] = 0;
	curr_row->len = index;
	new_row->next = curr_row->next;
	new_row->prev = curr_row;
	curr_row->next = new_row;
	if (new_row->next) {
		new_row->next->prev = new_row;
	}
	return FINE;
}

struct row_t* remove_row(struct row_t* row) {
	if (row == NULL) {
		return NULL;
	}
	struct row_t* old_row = row;
	if (row = old_row->prev) {
		row->next = old_row->next;
	} else if (row = old_row->next) {
		row->prev = old_row->prev;
	} else {
		row = NULL;
	}
	free_row(old_row);
	return row;
}

int jump_rows(struct row_t** row_p, int size) {
	if (row_p == NULL || *row_p == NULL) {
		return 0;
	}
	struct row_t* row = *row_p;
	int direction = size < 0 ? -1 : 1;
	size = direction > 0 ? size : -size;
	int total;
	for (total = 0; total < size; total++) {
		struct row_t* tmp;
		if (direction > 0) {
			tmp = row->next;
		} else {
			tmp = row->prev;
		}
		if (!tmp) {
			break;
		}
		row = tmp;
	}
	*row_p = row;
	return direction * total;
}

void print_snippet(struct row_t* row, int size) {
	if (row == NULL) {
		return;
	}
	int index = 0;
	for (; index > -size; index--) {
		if (row->prev == NULL) {
			break;
		}
		row = row->prev;
	}
	for (; index < size && row; index++) {
		if (index) {
			printf("%3d %s\n", index, row->text);
		} else {
			printf("->  %s\n", row->text);
		}
		row = row->next;
	}
}

void print_all_rows(struct row_t* row) {
	if (row == NULL) {
		return;
	}
	int i = 0;
	int j = 0;
	printf("0: '%s'\n", row->text);
	struct row_t * prev = row->prev;
	struct row_t * next = row->next;
	while(prev || next) {
		if (prev) {
			printf("%d: '%s'\n", --i, prev->text);
			prev = prev->prev;
		}
		if (next) {
			printf("%d: '%s'\n", ++j, next->text);
			next = next->next;
		}
	}
}

void print_all_rows_ordered(struct row_t* row, int line_numbers) {
	if (row == NULL) {
		return;
	}
	while (row->prev) {
		row = row->prev;
	}
	int i = 0;
	while(row) {
		if (line_numbers) {
			printf("%d\t", i);
		}
		printf("%s\n", row->text);
		row = row->next;
		i++;
	}
}

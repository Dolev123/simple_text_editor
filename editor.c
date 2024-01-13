#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "include/editor.h"
#include "include/utils.h"

#define DELIM " \t"
#define STRLN_PLUS(s) (strlen(s) + 1)
#define NEXT_TOKEN(sp) strtok_r(NULL, DELIM, sp)


Row* ignore (Row* row, char** savestrp) {
	return row;
}
Row* help (Row* row, char** savestrp);
Row* jump (Row* row, char** savestrp);
Row* print (Row* row, char** savestrp);
Row* replace (Row* row, char** savestrp);
Row* insert (Row* row, char** savestrp);
Row* delete (Row* row, char** savestrp);
Row* split (Row* row, char** savestrp);
Row* save (Row* row, char** savestrp);
Row* openfile (Row* row, char** savestrp);
Row* unknown (Row* row, char** savestrp){
	INFO("Unknown function given\n");
	return row;
}

typedef Row*(*cmd_function)(Row* row, char** savestrp);
static cmd_function cmd_to_function[] = {
	ignore   ,// exit
	help     ,// help
	jump     ,// jump
	print    ,// print
	replace  ,// replace
	insert   ,// insert
	delete   ,// delete
	split    ,// split
	save     ,// save
	openfile ,// open
	unknown  ,// unknown
};

static bool should_save = false;

int editor_loop(Row* start_row){
	Row* row = start_row;
	enum CMD cmd = UNKNOWN;
	
	while (cmd != EXIT) {
		// get line
		printf(":");
		char* line = NULL;
		size_t size = 0;
		ssize_t len = getline(&line, &size, stdin);
		if (len < 0) {
			ERROR("can't get line");
			continue;
		}
		line[--len] = 0;
		if (len == 0) {
			continue;
		}

		// parse line to cmd
		char* str = line + strspn(line, " \t");
		char* savestr;
		char* token = strtok_r(str, DELIM, &savestr);
		if (token == NULL) {
			// can happen
			continue;
		}
		cmd = string_to_cmd(token);
		row = cmd_to_function[cmd](row, &savestr);
		// free staff
		free(line);
	}
}

enum CMD string_to_cmd(const char* str) {
	if (strlen(str) == 1) {
		switch (str[0]) {
		case 'q':
			return EXIT;
		case 'h':
			return HELP;
		case 'j':
			return JUMP;
		case 'p':
			return PRINT;
		case 'r':
			return REPLACE;
		case 'i':
			return INSERT;
		case 'd':
			return DELETE;
		case 's':
			return SPLIT;
		case 'w':
			return SAVE;
		case 'o':
			return OPEN;
		default:
			return UNKNOWN;
		}
	}
	if (!strncmp("quit", str, STRLN_PLUS("quit"))
		|| !strncmp("exit", str, STRLN_PLUS("exit"))
	) {
		return EXIT;
	} else if (!strncmp("help", str, STRLN_PLUS("help"))) {
		return HELP;
	} else if (!strncmp("jump", str, STRLN_PLUS("jump"))) {
		return JUMP;
	} else if (!strncmp("print", str, STRLN_PLUS("print"))) {
		return PRINT;
	} else if (!strncmp("replace", str, STRLN_PLUS("replace"))) {
		return REPLACE;
	} else if (!strncmp("insert", str, STRLN_PLUS("insert"))) {
		return INSERT;
	} else if (!strncmp("delete", str, STRLN_PLUS("delete"))) {
		return DELETE;
	} else if (!strncmp("split", str, STRLN_PLUS("split"))) {
		return SPLIT;
	} else if (!strncmp("save", str, STRLN_PLUS("save"))) {
		return SAVE;
	} else if (!strncmp("open", str, STRLN_PLUS("open"))) {
		return OPEN;
	} else {
		return UNKNOWN;
	}
}

Row* help (Row* row, char** savestrp){
	// TODO:: add help for specific function
	printf("Available Commands:\n");
	for (int i = EXIT; i < UNKNOWN; i++) {
		printf("%2d: [%c] %s\n", i, cmd_to_char[i], cmd_to_string[i]);
	}
	return row;
}

Row* jump (Row* row, char** savestrp) {
	if (row == NULL) {
		ERROR("No file is loaded\n");
		return row;
	}
	int dst = 0;
	char* token = NEXT_TOKEN(savestrp);
	if (token == NULL || strlen(token) < 1) {
		ERROR("No destination given\n");
		return row;
	}
	// TODO:: check if token is a valid number
	switch (token[0]) {
	case '-':
		dst = -(atoi(token+1));
		
		break;
	case '+':
		dst = atoi(token+1);
		break;
	default:
		dst = atoi(token);
		row = traverse_to_start(row);
		break;
	}
	dst = jump_rows(&row, dst);
	return row;
}

Row* print (Row* row, char** savestrp){
	if (row == NULL) {
		ERROR("No file is loaded\n");
		return row;
	}
	int size = 0;
	char* token = NEXT_TOKEN(savestrp);
	if (token != NULL) {
		size = atoi(token);
	}
	size = size > 0 ? size : 5;
	print_snippet(row, size);
	return row;
}

Row* replace (Row* row, char** savestrp){
	if (row == NULL) {
		ERROR("No file is loaded\n");
		return row;
	}
	printf("> ");
	char* line = NULL;
	size_t size = 0;
	ssize_t len = getline(&line, &size, stdin);
	if (len < 0) {
		ERROR("can't get line");
		return row;
	}
	line[--len] = 0;
	free(row->text);
	row->text = line;
	row->len = len;
	should_save = true;
	return row;
}

Row* insert (Row* row, char** savestrp) {
	if (row == NULL) {
		ERROR("No file is loaded\n");
		return row;
	}
	printf("> ");
	char* line = NULL;
	size_t size = 0;
	ssize_t len = getline(&line, &size, stdin);
	if (len < 0) {
		ERROR("can't get line");
		return row;
	}
	line[--len] = 0;
	Row* row_new = new_row(line, len, row, row->next);
	if (row->next) {
		row->next->prev = row_new;
	}
	row->next = row_new;
	should_save = true;
	return row_new;
}

Row* delete (Row* row, char** savestrp){
	if (row == NULL) {
		ERROR("No file is loaded\n");
		return row;
	}
	// empty row if no other rows exist
	if (row->next == NULL && row->prev == NULL) {
		row->text = (char*)realloc(row->text, sizeof(char));
		row->text[0] = 0;
		row->len = 0;
		should_save = true;
		return row;
	}
	Row* row_pos;
	if (row->prev == NULL) {
		row_pos = row->next;
		row_pos->prev = row->prev;
	} else {
		row_pos = row->prev;
		row_pos->next= row->next;
	}
	free_row(row);
	should_save = true;
	return row_pos;
}

Row* split (Row* row, char** savestrp){
	if (row == NULL) {
		ERROR("No file is loaded\n");
		return row;
	}
	int index = -1;
	char* token = NEXT_TOKEN(savestrp);
	if (token != NULL && strlen(token) > 0) {
		index = atoi(token);
	}
	if (split_row(row, index) != FINE) {
		ERROR("Failed spliting row,\n");
	}
	should_save = true;
	return row;
}

Row* save (Row* row, char** savestrp) {
	if (row == NULL) {
		ERROR("No file is loaded\n");
		return row;
	}
	char* token = NEXT_TOKEN(savestrp);
	if (token == NULL) {
		// TODO:: get the current file's name
		ERROR("No path given\n");
		return row;
	}
	FILE* fp = fopen(token, "w");
	if (!fp) {
		ERROR("Can't open file: '%s'\n", token);
		return row;
	}
	Row* head = traverse_to_start(row);
	dump_rows_to_file(head, fp);
	fclose(fp);
	should_save = false;
	return row;
}

Row* openfile (Row* row, char** savestrp) {
	char* token = NEXT_TOKEN(savestrp);
	if (token == NULL) {
		ERROR("No path given\n");
		return row;
	}
	char* path = token;
	token = NEXT_TOKEN(savestrp);
	if (should_save && (token == NULL || strcmp(token, "!"))) {
		ERROR("Current open file has unsaved changes.\n");
		INFO("To discard those changes: '[o]pen <path> !'\n");
		return row;
	}
	free_all_rows(row);
	should_save = false;
	FILE* file = fopen(path, "r+");
	if (file == NULL) {
		ERROR("Failed opening file: %s\n", path);
		return NULL;
	}
	row = load_file_to_rows(file);
	if (row == NULL) {
		ERROR("failed loading file '%s'\n", path);
	}
	return traverse_to_start(row);
}

Row* load_file_to_rows(FILE* fp) {
	long old_pos = ftell(fp);
	if (fseek(fp, 0L, SEEK_SET)) {
		return NULL;
	}

	Row* row = NULL;
	char* line = NULL;
	size_t len = 0;
	ssize_t nread;
	while ((nread = getline(&line, &len, fp)) != -1) {
		if (!line) {
			goto fail_loading_file;
		}
		if (line[nread-1] == '\n') {
			line = (char*)realloc(line, nread);
			if (!line) {
				goto fail_loading_file;
			}
			line[--nread] = '\0';
		}
		Row* tmp = new_row(line, nread, row, NULL);
		if (!tmp) {
			goto fail_loading_file;
		}
		if (row) {
			row->next = tmp;
		}
		row = tmp;
		line = NULL;
                len = 0;
	}
	fseek(fp, old_pos, SEEK_SET);
	return row;
fail_loading_file:
	free_all_rows(row);
	return NULL;
}

size_t dump_rows_to_file(Row* row, FILE* fp) {
	if (!row) {
		return FINE;
	}
	if (fseek(fp, 0L, SEEK_SET)) {
		return ERR;
	}
	size_t total = 0;
	for (row; row != NULL; row=row->next) {
		size_t tmp = fwrite(row->text, 1, row->len, fp);
		total += tmp;
		if (tmp != row->len) {
			return ERR;
		}
		tmp = fwrite("\n", 1, 1, fp);
		total += tmp;
		if (tmp != 1) {
			return ERR;
		}
	}
	ftruncate(fileno(fp), total);
	return total;
}

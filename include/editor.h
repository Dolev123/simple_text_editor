#ifndef __EDITOR_H
#define __EDITOR_H

#include "rows.h"

enum CMD {
	EXIT,
	HELP,
	JUMP,
	PRINT,
	REPLACE,
	INSERT,
	DELETE,
	SPLIT,
	SAVE,
	OPEN,
	UNKNOWN
};

static const char cmd_to_char[] = {
	'q',
	'h',
	'j',
	'p',
	'r',
	'i',
	'd',
	's',
	'w',
	'o'
};
static const char* cmd_to_string[] = {
	"exit",
	"help",
	"jump",
	"print",
	"replace",
	"insert",
	"delete",
	"split",
	"save",
	"open",
	"unknown"
};
enum CMD string_to_cmd(const char* str);

int editor_loop(Row* start_row);
Row* load_file_to_rows();
size_t dump_rows_to_file(Row* row, FILE* fp);

#endif

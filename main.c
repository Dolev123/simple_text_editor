#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include "utils.h"
#include "printf_extentions.h"
#include "rows.h"
#include "editor.h"

#define FNAME_SIZE 256
#define VIEW 0
#define WRITE 1


int get_tty();
void print_usage(const char*);
int get_terminal_attributes();

// int Mode = VIEW;


void testing() {
	exit(0);
}


int main(const int argc, const char* argv[]) {
	int retval = FINE;
	FILE* file = NULL;
	const char* path;
	int tty_fd;
	struct termios term_config;

	RegisterPrintfFunctions();
	
	if (argc < 2) {
		print_usage(argv[0]);
		return FINE;
	}
	path = argv[1];
	if ((file = fopen(path, "r+")) == NULL) {
		ERROR("Failed opening file: %s\n", path);
		return ERR;
	}
	// tty_fd = get_tty();
	// get_terminal_attributes(tty_fd, &term_config);
	Row* row = load_file_to_rows(file);
	fclose(file);
	if (!row) {
		ERROR("Failed loading rows from file\n");
		retval = ERR;
		goto cleanup;
	}
#ifdef DEBUG_ON
	print_all_rows_ordered(row, PRINT_LN_IDX);
#endif
	row = traverse_to_start(row);
	editor_loop(row);

cleanup:
	INFO("Cleanup running\n");
	DEBUG("Freeing rows\n");
	free_all_rows(row);
	// DEBUG("Closing tty_fd\n");
	// close(tty_fd);
	return retval;
}


void print_usage(const char* prog){
	INFO("Usage: ./%s <file>\n", prog);
}

int get_terminal_attributes(int tty_fd, struct termios* term_config) {
	if (tcgetattr(tty_fd, term_config)) {
		ERROR("Failed getting terminal attributes\n");
		return ERR;
	}
	DEBUG("c_Iflag: %B\n", term_config->c_iflag);
	DEBUG("c_Oflag: %B\n", term_config->c_oflag);
	DEBUG("c_Cflag: %B\n", term_config->c_cflag);
	DEBUG("c_Lflag: %B\n", term_config->c_lflag);
	return FINE;
}

int get_tty() {
	char fd_path[1024] = {0};
	readlink("/proc/self/fd/0", fd_path, ARRAY_SIZE(fd_path));
	DEBUG("fd_path: %s\n", fd_path);
	int fd = open((const char *)fd_path, O_RDWR|O_NOCTTY|O_NDELAY);
	if (fd == ERR) {
		ERROR("could not open tty at: %s\n", fd_path);
		exit(ERR);
	}
	DEBUG("opened tty with fd: %d\n", fd);
	return fd;
}



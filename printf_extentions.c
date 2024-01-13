#include <limits.h>

#include "printf_extentions.h"
#include "utils.h"

int printf_arginfo_binary(
		const struct printf_info *info,
		size_t n,
		int argtypes[],
		int size[]
	) {
	if (n < 1) {
               return ERR;
	}
	if (info->is_long_double) {
		argtypes[0] = PA_INT | PA_FLAG_LONG_LONG;
	} else if (info->is_long) {
		argtypes[0] = PA_INT | PA_FLAG_LONG;
	} else {
		argtypes[0] = PA_INT;
	}
	return 1;
}

int printf_output_binary (
		FILE* stream,
		const struct printf_info* info,
		const void* const args[]
	) {
	int size = 0;
	unsigned long long number = *(unsigned long long *)(args[0]);
	if (info->is_long_double) {
	} if (info->is_long) {
		number = (long)number;
	} else {
		number = (int)number;
	}
	if (number <= UCHAR_MAX) {
		size = sizeof(char)*8;
	} else if (number <= USHRT_MAX) {
		size = sizeof(short)*8;
	} else if (number <= UINT_MAX) {
		size = sizeof(char)*8;
	} else {
		size = sizeof(long)*8;
	}
	char buff[size+1] = {};
	for (int i = 0; i < size; i++) {
		buff[size-i-1] = '0' + (number %2 );
		number >>= 1;
	}
	buff[size] = 0;
	return fprintf(stream, "%s", buff);
}

int RegisterPrintfFunctions() {
	if (register_printf_specifier('B', 
		printf_output_binary, 
		printf_arginfo_binary
	)) {
		ERROR("Failed to register printf binary functions\n");
		return ERR;
	}
	DEBUG("Successfully registered printf binary.\n");
	return FINE;
}


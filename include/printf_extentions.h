#ifndef __PRINTF_H
#define __PRINTF_H

#include <stdio.h>
#include <stddef.h>
#include <printf.h>

#define PA_BIN PA_LAST

int printf_arginfo_binary(
		const struct printf_info *info,
		size_t n,
		int argtypes[],
		int size[]
	);
int printf_output_binary (
		FILE* stream,
		const struct printf_info* info,
		const void* const args[]
	);
int RegisterPrintfFunctions();

#endif

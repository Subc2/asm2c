/* asm2c - converts assembler source code into C source code
 * Copyright (C) 2014-2016 Paweł Zacharek
 * 
 * -----------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * -----------------------------------------------------------------------
 * 
 * date: 2016-10-25
 * compiling: gcc -std=gnu11 -o asm2c.elf asm2c.c
 */

#include <errno.h>
#include <getopt.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int buf[2], option;
	char *input_file = NULL, *output_file = NULL;
	FILE *input, *output;
	const struct option LongOptions[] = {
		{"help",   0, NULL, 'h'},
		{"output", 1, NULL, 'o'},
		{NULL, 0, NULL, 0}
	};

	while ((option = getopt_long(argc, argv, "ho:", LongOptions, NULL)) != -1)
		switch (option) {
			case 'h':
				puts("asm2c - converts assembler source code into C source code\n"
				"Syntax: asm2c [-h|--help] [-o OUTPUT_FILE|--output OUTPUT_FILE] [INPUT_FILE]");
				return 0;
			case 'o':
				output_file = optarg;
				break;
			default:
				return EINVAL;
		}
	if (optind == argc - 1)  // 1 unknown parameter (input file name)
		input_file = argv[optind];
	else if (optind != argc) {  // more unknown parameters
		fputs("Invalid argument(s) found.\n", stderr);
		return EINVAL;
	}

	if ((input = (input_file ? fopen(input_file, "r") : stdin)) == NULL) {
		perror(input_file);
		return ENOENT;
	}
	if ((output = (output_file ? fopen(output_file, "w") : stdout)) == NULL) {
		perror(output_file);
		return ENOENT;
	}

	fputs("asm(\"", output);
	for (buf[0] = '\0'; (buf[1] = getc(input)) != EOF; buf[0] = buf[1]) {
		if (buf[0] == '\n')  // there's another character after '\n'
			fputs("\\n\"\n\"", output);  // ends old and starts new line
		if (buf[1] != '\n') {
			if (buf[1] == '\"' || buf[1] == '\\')
				putc('\\', output);
			putc(buf[1], output);
		}
	}
	fclose(input);
	fputs("\");", output);
	if (buf[0] == '\n')  // the last input character is '\n'
		putc('\n', output);
	fclose(output);

	return 0;
}

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "print.h"
#include "read.h"
#include "types.h"

/***************************** FUNCTIONS ******************************/

void init() {}

/*************************** EVALUATE ****************************/

/* until we have lists and symbols just echo */
Object *eval(Object *exp) { return exp; }

/***************************** REPL ******************************/

int main(void) {
	Object *value;
	int c;

	printf("Welcome to Bootstrap Scheme. "
		   "Use ctrl-c to exit.\n");

	init();

	while (1) {
		printf("> ");
		fflush(stdout);
		skip_whitespace(stdin);
		value = eval(read_object(stdin));

		bool line_empty = true;
		while ((c = getc(stdin)) != '\n') {
			if (!is_whitespace(c)) {
				line_empty = false;
			}
		}
		print_object(stdout, value);
		printf("\n");
		fflush(stdout);

		if (!line_empty) {
			fprintf(stderr, "Unexpected input after value was read\n");
			fflush(stderr);
		}
	}

	return 0;
}

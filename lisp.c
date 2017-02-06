#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "read.h"
#include "print.h"

/***************************** FUNCTIONS ******************************/
	
void init(){
	
}


/*************************** EVALUATE ****************************/

/* until we have lists and symbols just echo */
Object *eval(Object *exp) {
    return exp;
}

/***************************** REPL ******************************/

int main(void) {
	Object *value;
	
    printf("Welcome to Bootstrap Scheme. "
           "Use ctrl-c to exit.\n");

    init();

    while (1) {
        printf("> ");
		fflush(stdout);
		eat_whitespace(stdin);
		value = eval(read_object(stdin));
		while ( getc(stdin) != '\n' ) {}
		print_object(stdout, value);
        printf("\n");
		fflush(stdout);
    }

    return 0;
}


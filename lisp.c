#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "make.h"
#include "read.h"

/***************************** FUNCTIONS ******************************/
	
void init(){
	
}


/*************************** EVALUATE ****************************/

/* until we have lists and symbols just echo */
Object *eval(Object *exp) {
    return exp;
}

/**************************** PRINT ******************************/



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
		value = eval(read_value(stdin));
		while ( getc(stdin) != '\n' ) {}
		write(value);
        printf("\n");
		fflush(stdout);
    }

    return 0;
}

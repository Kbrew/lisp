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
	int c;
	
    printf("Welcome to Bootstrap Scheme. "
           "Use ctrl-c to exit.\n");

    init();

    while (1) {
        printf("> ");
		fflush(stdout);
		eat_whitespace(stdin);
		value = eval(read_object(stdin));
		
		bool line_empty = true;
		while ( (c = getc(stdin)) != '\n' ) {
			if(!is_whitespace(c)){
				line_empty = false;
			}
		}
		print_object(stdout, value);
        printf("\n");
		
		if(!line_empty){
			printf("Unexpected input after value was read\n");
		}
		
		fflush(stdout);
    }

    return 0;
}


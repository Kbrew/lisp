#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "make.h"
#include "read.h"

/***************************** FUNCTIONS ******************************/
	
bool is_boolean(Object *obj) {
    return obj->type == BOOLEAN;
}

bool is_false(Object *obj) {
    return obj->boolean.value == false;
}

char is_true(Object *obj) {
    return !(obj->boolean.value == false);
}

void init(){
	
}


/*************************** EVALUATE ****************************/

/* until we have lists and symbols just echo */
Object *eval(Object *exp) {
    return exp;
}

/**************************** PRINT ******************************/

void write(Object *obj) {
	char *str_ptr;
	
    switch (obj->type) {
        case BOOLEAN:
            printf("#%c", is_false(obj) ? 'f' : 't');
            break;
        case INTEGER:
            printf("%d", obj->integer.value);
            break;
        case STRING:
            str_ptr = obj->string.value;
            putchar('"');
            while (*str_ptr != '\0') {
                switch (*str_ptr) {
                    case '\n':
                        printf("\\n");
                        break;
                    case '\t':
                        printf("\\t");
                        break;
					case '\r':
                        printf("\\r");
                        break;
                    case '\\':
                        printf("\\\\");
                        break;
                    case '"':
                        printf("\\\"");
                        break;
                    default:
                        putchar(*str_ptr);
                }
                str_ptr++;
            }
            putchar('"');
			break;
		case SYMBOL:
            str_ptr = obj->string.value;
			printf("%s", str_ptr);
			break;
		case CONS:
			putchar('(');
			write(obj->cons.head);
			
			Object *tail;
			tail = obj->cons.tail;
			while(tail->type == CONS){
				putchar(' ');
				write(tail->cons.head);
				tail = tail->cons.tail;
			}
			if(tail->type != NIL){
				printf(" . ");
				write(tail);
			}
			putchar(')');
			break;
		case NIL:
			printf("()");
			break;
        default:
            fprintf(stderr, "cannot write unknown type\n");
            exit(1);
    }
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
		value = eval(read_value(stdin));
		while ( getc(stdin) != '\n' ) {}
		write(value);
        printf("\n");
		fflush(stdout);
    }

    return 0;
}

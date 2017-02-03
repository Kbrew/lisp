#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


#define BUFFER_MAX 1024
/***************************** TYPES ******************************/

typedef enum
{
	BOOLEAN,
	INTEGER,
	SYMBOL,
	STRING,
	CONS,
	NIL,
} ObjectType;

typedef union Object Object;

typedef struct Integer
{
	ObjectType type;
	int value;
} Integer;

typedef struct Boolean
{
	ObjectType type;
	bool value;
} Boolean;

typedef struct Symbol
{
	ObjectType type;
	char *value;
} Symbol;

typedef struct String
{
	ObjectType type;
	char *value;
} String;

typedef struct Cons
{
	ObjectType type;
	Object* head;
	Object* tail;
} Cons;

typedef struct Nil
{
	ObjectType type;
} Nil;

typedef union Object
{
	ObjectType type;
	Integer integer;
	Boolean boolean;
	Symbol symbol;
	String string;
	Cons cons;
	Nil nil;
} Object;



/***************************** FUNCTIONS ******************************/

Object *alloc_object(void) {
    Object *obj;

    obj = malloc(sizeof(Object));
    if (obj == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    return obj;
}

Object *make_integer(int value){
	Object *obj = alloc_object();
	obj->type = INTEGER;
	obj->integer.value = value;
	
	return obj;
}

Object *make_boolean(bool value){
	Object *obj = alloc_object();
	obj->type = BOOLEAN;
	obj->boolean.value = value;
	
	return obj;
}

Object *make_string(char *value){
	Object *obj = alloc_object();
	char *new_string = malloc(strlen(value) + 1);
    
	if (new_string == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
	}
	strcpy(new_string, value);
	
	obj->type = STRING;
	obj->string.value = new_string;
	
	return obj;
}

Object *make_symbol(char *value){
	Object *obj;
	
	// strings and symbols are identical except the tag.
	// so we cheat and reuse the string constructor
	obj = make_string(value);
	obj->type = SYMBOL;
	
	return obj;
}

Object *make_cons(Object* head, Object* tail){
	Object *obj = alloc_object();
	
	obj->type = CONS;
	obj->cons.head = head;
	obj->cons.tail = tail;
	
	return obj;
}

Object *make_nil(){
	Object *obj = alloc_object();
	
	obj->type = NIL;

	return obj;
}
	
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

/***************************** READ ******************************/

bool is_open_paren(char c){
	return
		c == '(' ||
		c == '[' ||
		c == '{';
}

bool is_close_paren(char c){
	return
		c == ')' ||
		c == ']' ||
		c == '}';
}

bool is_matching_paren(char c1, char c2){
	switch(c1){
		case '(':
			return c2 == ')';
		case '[':
			return c2 == ']';
		case '{':
			return c2 == '}';
		default:
			return false;
	}
}

char is_delimiter(int c) {
    return isspace(c) ||
	       is_open_paren(c) ||
		   is_close_paren(c) ||
		   c == '"' ||
		   c == ';' ;
}

int peek(FILE *in) {
    int c;

    c = getc(in);
    ungetc(c, in);
    return c;
}

void eat_whitespace(FILE *in) {
    int c;
    
    while ((c = getc(in)) != EOF) {
        if (isspace(c)) {
            continue;
        }
        else if (c == ';') { /* comments are whitespace also */
            while (((c = getc(in)) != EOF) && (c != '\n'));
            continue;
        }
        ungetc(c, in);
        break;
    }
}

Object *read_boolean(FILE *in)
{
	int c;
	
	c = getc(in);
	if(c != '#'){
		fprintf(stderr, "illegal boolean literal, expected '#' found '%c'\n", c);
		exit(1);
	}
	
	c = getc(in);
	switch (c) {
	case 't':
		return make_boolean(true);
	case 'f':
		return make_boolean(false);
	default:
		fprintf(stderr, "unknown boolean literal, expected 't' or 'f' found '%c'\n", c);
		exit(1);
	}
}

Object *read_integer(FILE *in) {
    int c;
    short sign;
    long num = 0;

    c = getc(in); 

	if (c == '-') {
		sign = -1;
	}
	else {
		sign = 1;
		ungetc(c, in);
	}
	
	while (isdigit(c = getc(in))) {
		num = (num * 10) + (c - '0');
	}
	
	num *= sign;
	if (is_delimiter(c)) {
		ungetc(c, in);
		return make_integer(num);
	} else {
		fprintf(stderr, "number not followed by delimiter\n");
		exit(1);
	}
}

char read_escaped_string_char(FILE *in)
{
	int c = getc(in);
	
	if (c == EOF) {
		fprintf(stderr, "non-terminated string literal\n");
		exit(1);
	}else if(c == '\\'){
		c = getc(in);
		switch(c){
			case 'n':
				return '\n';
			case 't':
				return '\t';
			case 'r':
				return '\r';
			default:
				return c;
		}
	} else {
		return c;
	}
}

Object *read_string(FILE *in)
{
	int c;
	int len;
	char buffer[BUFFER_MAX];
	
	c = getc(in);
	if(c != '"'){
		fprintf(stderr, "illegal string literal, expected '\"' found '%c'\n", c);
		exit(1);
	}
	
	len = 0;
	while ((c = peek(in)) != '"') {
		c = read_escaped_string_char(in);

        /* subtract 1 to save space for '\0' terminator */
        if (len < BUFFER_MAX - 1) {
            buffer[len] = c;
			len += 1;
		}
		else {
			fprintf(
				stderr, 
				"string too long. Maximum length is %d\n",
				BUFFER_MAX);
			exit(1);
        }
        buffer[len] = '\0';
	}
	getc(in); //drain the peeked '"'
	
	return make_string(buffer);
}

bool is_symbol_char(char c)
{
	return !is_delimiter(c);
}

Object *read_symbol(FILE *in)
{
	int c;
	int len;
	char buffer[BUFFER_MAX];
	
	c = peek(in);
	if(!is_symbol_char(c)){
		fprintf(stderr, "illegal symbol char, found '%c'\n", c);
		exit(1);
	}
	
	len = 0;
	while (is_symbol_char(c = getc(in))) {
        /* subtract 1 to save space for '\0' terminator */
        if (len < BUFFER_MAX - 1) {
            buffer[len] = c;
			len += 1;
		}
		else {
			fprintf(
				stderr, 
				"string too long. Maximum length is %d\n",
				BUFFER_MAX);
			exit(1);
        }
        buffer[len] = '\0';
	}
	ungetc(c, in); //drain the peeked '"'
	
	return make_symbol(buffer);	
}

Object *read_value(FILE *in);

Object *read_list_tail(FILE *in)
{
	Object *head, *tail;
	eat_whitespace(in);
	if(is_close_paren(peek(in))){
		return make_nil();
	} else {
		head = read_value(in);
		tail = read_list_tail(in);
		return make_cons(head, tail);
	}
}

Object *read_list(FILE *in){
	int open_paren, close_paren;
	Object *obj;
	
	open_paren = getc(in);
	if( !is_open_paren(open_paren) ){
		printf("invalid opening paren '%c'", open_paren);
		exit(1);
	}
	
	obj = read_list_tail(in);
	
	close_paren = getc(in);
	if( !is_matching_paren(open_paren, close_paren) ){
		printf("non matching parens '%c' and '%c'", open_paren, close_paren);
		exit(1);
	}
	
	return obj;
}
	

Object *read_value(FILE *in)
{
	int c = peek(in);
	
	if(c == '#'){
		return read_boolean(in);
	}else if(isdigit(c) || (c == '-')){
		return read_integer(in);
	}else if(c == '"'){
		return read_string(in);
	}else if(is_open_paren(c)){
		return read_list(in);
	}else{
        return read_symbol(in);
	}
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

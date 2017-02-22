#include "read.h"

#include <ctype.h>
#include <stdlib.h>

#include "make.h"

#define BUFFER_MAX 1024

bool is_whitespace(int c) { return isspace(c); }

bool is_open_paren(int c) { return c == '(' || c == '[' || c == '{'; }

bool is_close_paren(int c) { return c == ')' || c == ']' || c == '}'; }

bool is_matching_paren(int c1, int c2) {
	switch (c1) {
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

bool is_delimiter(int c) {
	return is_whitespace(c) || is_open_paren(c) || is_close_paren(c) ||
		   c == '"' || c == ';';
}

int peek(FILE *in) {
	int c;

	c = getc(in);
	ungetc(c, in);
	return c;
}

void skip_whitespace(FILE *in) {
	int c;

	while ((c = getc(in)) != EOF) {
		if (is_whitespace(c)) {
			continue;
		} else if (c == ';') { /* comments are whitespace also */
			while (((c = getc(in)) != EOF) && (c != '\n'))
				;
			continue;
		}
		ungetc(c, in);
		break;
	}
}

Object *read_boolean(FILE *in) {
	int c;

	c = getc(in);
	if (c != '#') {
		fprintf(stderr, "illegal boolean literal, expected '#' found '%c'\n",
				c);
		exit(1);
	}

	c = getc(in);
	switch (c) {
	case 't':
		return make_boolean(true);
	case 'f':
		return make_boolean(false);
	default:
		fprintf(stderr,
				"unknown boolean literal, expected 't' or 'f' found '%c'\n", c);
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
	} else {
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

char read_escaped_string_char(FILE *in) {
	int c = getc(in);

	if (c == EOF) {
		fprintf(stderr, "non-terminated string literal\n");
		exit(1);
	} else if (c == '\\') {
		c = getc(in);
		switch (c) {
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

Object *read_string(FILE *in) {
	int c;
	int len;
	char buffer[BUFFER_MAX];

	c = getc(in);
	if (c != '"') {
		fprintf(stderr, "illegal string literal, expected '\"' found '%c'\n",
				c);
		exit(1);
	}

	len = 0;
	while ((c = peek(in)) != '"') {
		c = read_escaped_string_char(in);

		/* subtract 1 to save space for '\0' terminator */
		if (len < BUFFER_MAX - 1) {
			buffer[len] = c;
			len += 1;
		} else {
			fprintf(stderr, "string too long. Maximum length is %d\n",
					BUFFER_MAX);
			exit(1);
		}
		buffer[len] = '\0';
	}
	getc(in); // drain the peeked '"'

	return make_string(buffer);
}

bool is_symbol_char(int c) { return !is_delimiter(c); }

Object *read_symbol(FILE *in) {
	int c;
	int len;
	char buffer[BUFFER_MAX];

	c = peek(in);
	if (!is_symbol_char(c)) {
		fprintf(stderr, "illegal symbol char, found '%c'\n", c);
		exit(1);
	}

	len = 0;
	while (is_symbol_char(c = getc(in))) {
		/* subtract 1 to save space for '\0' terminator */
		if (len < BUFFER_MAX - 1) {
			buffer[len] = c;
			len += 1;
		} else {
			fprintf(stderr, "string too long. Maximum length is %d\n",
					BUFFER_MAX);
			exit(1);
		}
		buffer[len] = '\0';
	}
	ungetc(c, in); // drain the peeked '"'

	return make_symbol(buffer);
}

Object *read_list_tail(FILE *in) {
	Object *head, *tail;
	skip_whitespace(in);
	if (is_close_paren(peek(in))) {
		return make_nil();
	} else {
		head = read_object(in);
		tail = read_list_tail(in);
		return make_cons(head, tail);
	}
}

Object *read_list(FILE *in) {
	int open_paren, close_paren;
	Object *obj;

	open_paren = getc(in);
	if (!is_open_paren(open_paren)) {
		printf("invalid opening paren '%c'", open_paren);
		exit(1);
	}

	obj = read_list_tail(in);

	close_paren = getc(in);
	if (!is_matching_paren(open_paren, close_paren)) {
		printf("non matching parens '%c' and '%c'", open_paren, close_paren);
		exit(1);
	}

	return obj;
}

Object *read_object(FILE *in) {
	int c = peek(in);

	if (c == '#') {
		return read_boolean(in);
	} else if (isdigit(c) || (c == '-')) {
		return read_integer(in);
	} else if (c == '"') {
		return read_string(in);
	} else if (is_open_paren(c)) {
		return read_list(in);
	} else {
		return read_symbol(in);
	}
}

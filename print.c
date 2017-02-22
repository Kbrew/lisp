#include "print.h"

#include <stdlib.h>

void print_object(FILE *out, Object *obj) {
	switch (obj->type) {
	case BOOLEAN:
		print_boolean(out, obj);
		break;
	case INTEGER:
		print_integer(out, obj);
		break;
	case STRING:
		print_string(out, obj);
		break;
	case SYMBOL:
		print_symbol(out, obj);
		break;
	case CONS:
		print_list(out, obj);
		break;
	case NIL:
		print_nil(out, obj);
		break;
	default:
		fprintf(stderr, "cannot write unknown type\n");
		exit(1);
	}
}

void print_boolean(FILE *out, Object *obj) {
	fprintf(out, "#%c", obj->boolean.value ? 't' : 'f');
}

void print_integer(FILE *out, Object *obj) {
	fprintf(out, "%d", obj->integer.value);
}

void print_string(FILE *out, Object *obj) {
	char *str_ptr;

	str_ptr = obj->string.value;
	fputc('"', out);
	while (*str_ptr != '\0') {
		switch (*str_ptr) {
		case '\n':
			fprintf(out, "\\n");
			break;
		case '\t':
			fprintf(out, "\\t");
			break;
		case '\r':
			fprintf(out, "\\r");
			break;
		case '\\':
			fprintf(out, "\\\\");
			break;
		case '"':
			fprintf(out, "\\\"");
			break;
		default:
			fputc(*str_ptr, out);
		}
		str_ptr++;
	}
	fputc('"', out);
}

void print_list(FILE *out, Object *obj) {
	fputc('(', out);
	print_object(out, obj->cons.head);

	Object *tail;
	tail = obj->cons.tail;
	while (tail->type == CONS) {
		fputc(' ', out);
		print_object(out, tail->cons.head);
		tail = tail->cons.tail;
	}
	if (tail->type != NIL) {
		fprintf(out, " . ");
		print_object(out, tail);
	}
	fputc(')', out);
}

void print_symbol(FILE *out, Object *obj) {
	fprintf(out, "%s", obj->string.value);
}

void print_nil(FILE *out, Object *obj) { 
	fprintf(out, "()");
}

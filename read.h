#pragma once

#include <stdio.h>
#include "types.h"

int peek(FILE *in);
void eat_whitespace(FILE *in);
char read_escaped_string_char(FILE *in);

bool is_whitespace(int c);
bool is_delimiter(int c);
bool is_symbol_char(int c);
bool is_open_paren(int c);
bool is_close_paren(int c);
bool is_matching_paren(int c1, int c2);

Object *read_object(FILE *in);
Object *read_boolean(FILE *in);
Object *read_integer(FILE *in);
Object *read_symbol(FILE *in);
Object *read_string(FILE *in);
Object *read_list(FILE *in);

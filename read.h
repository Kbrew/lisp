#pragma once

#include "types.h"
#include "parse_stream.h"
#include <stdio.h>


char read_escaped_string_char(ParseStream *in);

bool is_whitespace(int c);
bool is_delimiter(int c);
bool is_symbol_char(int c);
bool is_open_paren(int c);
bool is_close_paren(int c);
bool is_matching_paren(int c1, int c2);

Object *read_object(ParseStream *in);
Object *read_boolean(ParseStream *in);
Object *read_integer(ParseStream *in);
Object *read_symbol(ParseStream *in);
Object *read_string(ParseStream *in);
Object *read_list(ParseStream *in);

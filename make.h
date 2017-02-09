#pragma once

#include "types.h"

Object *alloc_object(void);

Object *make_integer(int value);
Object *make_boolean(bool value);
Object *make_string(char *value);
Object *make_symbol(char *value);
Object *make_cons(Object *head, Object *tail);
Object *make_nil();

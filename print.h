#pragma once

#include <stdio.h>

#include "types.h"

#include "print.h"



void print_object(FILE *out, Object *obj);
void print_boolean(FILE *out, Object *obj);
void print_integer(FILE *out, Object *obj);
void print_string(FILE *out, Object *obj);
void print_list(FILE *out, Object *obj);
void print_symbol(FILE *out, Object *obj);
void print_nil(FILE *out, Object *obj);
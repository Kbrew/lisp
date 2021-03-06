#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "uthash.h"

typedef enum {
	BOOLEAN,
	INTEGER,
	SYMBOL,
	STRING,
	CONS,
	NIL,
	PRIMATIVE_FORM
} ObjectType;

typedef union Object Object;

typedef struct Integer {
	ObjectType type;
	int value;
} Integer;

typedef struct Boolean {
	ObjectType type;
	bool value;
} Boolean;

typedef struct Symbol {
	ObjectType type;
	char *value;
} Symbol;

typedef struct String {
	ObjectType type;
	char *value;
} String;

typedef struct Cons {
	ObjectType type;
	Object *head;
	Object *tail;
} Cons;

typedef struct Nil { ObjectType type; } Nil;

typedef union Object {
	ObjectType type;
	Integer integer;
	Boolean boolean;
	Symbol symbol;
	String string;
	Cons cons;
	Nil nil;
} Object;

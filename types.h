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

// typedef void primative_form_fn(Context *ctx, Object *args);

// typedef struct PrimativeForm {
	// ObjectType type;
	// primative_form_fn* fn;
// } PrimativeForm;

typedef struct SourceLocation{
	char *file_name;
	unsigned int line;
	unsigned int col;
} SourceLocation;

typedef struct Scope {
	unsigned int id;
	char *name;
	SourceLocation loc;
} Scope;

typedef struct ScopeSet{
	size_t size;
	Scope scopes[];
} ScopeSet;

typedef struct Syntax {
	Object *value;
	SourceLocation loc;
	ScopeSet *scope_set;
} Syntax;	

typedef struct ParseStream {
	FILE *file;
	SourceLocation loc;
} ParseStream;

typedef union Object {
	ObjectType type;
	Integer integer;
	Boolean boolean;
	Symbol symbol;
	String string;
	Cons cons;
	Nil nil;
} Object;

bool is_boolean(Object *obj);
bool is_false(Object *obj);
bool is_true(Object *obj);

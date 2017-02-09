#include <stdbool.h>

typedef enum {
	NIL,
	BOOLEAN,
	SYMBOL,
	FIXNUM,
	CHARACTER,
	STRING,
	PAIR,
	PRIMITIVE_PROC,
	COMPOUND_PROC,
	INPUT_PORT,
	OUTPUT_PORT,
	EOF_OBJECT,
} ObjectType;

typedef struct Boolean {
	ObjectType tag;
	bool value;
} Boolean;

typedef struct Symbol {
	ObjectType tag;
	char *value;
} Symbol;

typedef struct FixNum {
	ObjectType tag;
	long value;
} FixNum;

typedef struct Character {
	ObjectType tag;
	char value;
} Character;

typedef struct String {
	ObjectType tag;
	char *value;
} String;

typedef union {
	ObjectType tag;
	Boolean boolean;
	Symbol symbol;
	FixNum fixnum;
	Character character;
	String string;
} Object;

Object get_object(bool make_nil) {
	Object obj;
	if (make_nil) {
		obj.tag = NIL;
	} else {
		obj.tag = BOOLEAN;
		obj.boolean.value = true;
	}

	return obj;
}

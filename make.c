#include "make.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

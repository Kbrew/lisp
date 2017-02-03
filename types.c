#include "types.h"

bool is_boolean(Object *obj) {
    return obj->type == BOOLEAN;
}

bool is_false(Object *obj) {
    return !obj->boolean.value;
}

bool is_true(Object *obj) {
    return obj->boolean.value;
}

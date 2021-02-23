#include <asl_core.h>
#include <asl_value.h>

VALUE_TYPE_MAP_DEF(int) = VALUE_TYPE_MAP_BODY(int, value_type_integer);
VALUE_TYPE_MAP_DEF(integer) = VALUE_TYPE_MAP_BODY(integer, value_type_integer);
VALUE_TYPE_MAP_DEF(double) = VALUE_TYPE_MAP_BODY(double, value_type_double);
VALUE_TYPE_MAP_DEF(float) = VALUE_TYPE_MAP_BODY(float, value_type_double);
VALUE_TYPE_MAP_DEF(string) = VALUE_TYPE_MAP_BODY(string, value_type_string);
VALUE_TYPE_MAP_DEF(bool) = VALUE_TYPE_MAP_BODY(string, value_type_bool);
VALUE_TYPE_MAP_DEF(array) = VALUE_TYPE_MAP_BODY(array, value_type_array);

value_type_map_t *global_value_type_map[] = {
        VALUE_TYPE_MAP_PNAME(int),
        VALUE_TYPE_MAP_PNAME(integer),
        VALUE_TYPE_MAP_PNAME(double),
        VALUE_TYPE_MAP_PNAME(float),
        VALUE_TYPE_MAP_PNAME(string),
        VALUE_TYPE_MAP_PNAME(bool),
        VALUE_TYPE_MAP_PNAME(array),
        NULL
};

value_info_t global_value = {
        NULL,
        global_value_type_map
};

/**
 *
 * @return
 */
int value_init() {
    value_type_map_t *vtm;
    global_value.type_map_table = hash_create();
    vtm = global_value.type_map[0];
    while (vtm) {
        if (is_empty(vtm) || vtm->type == 0 || strlen(vtm->name) == 0) {
            break;
        }
        hash_insert_or_update(global_value.type_map_table, vtm->name, strlen(vtm->name), &vtm->type);
        vtm++;
    }

}

/**
 *
 * @param list
 * @return
 */
int value_destroy_list(value_list_t *list) {
    value_list_item_t *item, *free_item;
    if (is_empty(list)) {
        return RET_SUCCESS;
    }
    if (list->count == 0) {
        memory_free(list);
        return RET_SUCCESS;
    }
    item = list->head;
    while (item) {
        if (item->v->refer_count == 0) {
            output_debug("value_free call 13");
            value_free(item->v);
        }
        free_item = item;
        item = item->next;
        memory_free(free_item);
    }
    memory_free(list);
}

/**
 * 创建参数链表
 * @param p
 * @return
 */
value_list_t *value_create_list() {
    value_list_t *list;
    list = (value_list_t *) memory_alloc(sizeof(value_list_t));
    list->head = list->tail = NULL;
    list->count = 0;
    return list;
}

/**
 * 插入参数链表
 * @param list
 * @param p
 * @return
 */
value_list_t *value_insert_list(value_list_t *list, value_t *v) {
    value_list_item_t *item;
    item = (value_list_item_t *) memory_alloc(sizeof(value_list_item_t));
    item->v = v;
    item->next = NULL;
    if (list->count == 0) {
        list->tail = list->head = item;
    } else {
        list->tail->next = item;
        list->tail = item;
    }
    list->count++;
    return list;
}

/**
 * 创建null值
 * @return
 */
value_t *value_create_null() {
    value_t *v;
    v = (value_t *) memory_alloc(sizeof(value_t));
    v->type = value_type_null;
    v->refer = v->refer_count = 0;
    v->u.n = TRUE_VAL;
    return v;
}

value_t *value_create_double(double d) {
    value_t *v;
    v = value_create_null();
    v->type = value_type_double;
    v->u.d = d;
    return v;
}

value_t *value_create_integer(int i) {
    value_t *v;
    v = value_create_null();
    v->type = value_type_integer;
    v->u.i = i;
    return v;
}

value_t *value_create_string(char *s) {
    value_t *v;
    v = value_create_null();
    v->type = value_type_string;
    v->u.str.val = s;
    v->u.str.len = strlen(s);
    return v;
}

value_t *value_create_bool(char b) {
    value_t *v;
    v = value_create_null();
    v->type = value_type_bool;
    v->u.b = b;
    return v;
}

int value_incr_quote(value_t *v) {
    if (is_empty(v)) {
        return RET_FAILURE;
    }
    v->refer_count++;
    return RET_SUCCESS;
}

/**
 *
 * @param v
 * @return
 */
int value_decr_quote(value_t *v) {
    if (is_empty(v)) {
        return RET_FAILURE;
    }
    v->refer_count--;
    if (v->refer_count <= 0) {
        value_free(v);
    }
    return RET_SUCCESS;
}

int value_free(value_t *v) {
    if (is_empty(v)) {
        return RET_FAILURE;
    }
    if (v->type == value_type_string) {
        memory_free(v->u.str.val);
    }
    memory_free(v);
    return RET_SUCCESS;
}

value_t *value_transfer_int(value_t *v) {
    char *s;
    switch (v->type) {
        case value_type_bool:
            v->u.i = (int) v->u.b;
            break;
        case value_type_double:
            v->u.i = (int) v->u.d;
            break;
        case value_type_string:
            s = v->u.str.val;
            v->u.i = atoi(v->u.str.val);
            memory_free(s);
            break;
        case value_type_null:
            v->u.i = 0;
            break;
        case value_type_integer:
            break;
        default:
            v->u.i = 0;
            break;
    }
    v->type = value_type_integer;
    return v;
}

value_t *value_transfer_double(value_t *v) {
    char *s;
    switch (v->type) {
        case value_type_bool:
            v->u.d = (double) v->u.b;
            break;
        case value_type_integer:
            v->u.d = (double) v->u.i;
            break;
        case value_type_string:
            s = v->u.str.val;
            v->u.d = (double) atoi(s);
            memory_free(s);
            break;
        case value_type_null:
            v->u.d = 0;
            break;
        default:
            v->u.d = 0;
            break;
    }
    v->type = value_type_double;
}

value_t *value_transfer_bool(value_t *v) {
    char *s;
    switch (v->type) {
        case value_type_double:
            v->u.b = v->u.d ? TRUE_VAL : FALSE_VAL;
            break;
        case value_type_integer:
            v->u.b = v->u.d ? TRUE_VAL : FALSE_VAL;
            break;
        case value_type_string:
            s = v->u.str.val;
            v->u.b = v->u.str.val ? TRUE_VAL : FALSE_VAL;
            memory_free(s);
            break;
        case value_type_null:
            v->u.b = FALSE_VAL;
            break;
        case value_type_bool:
            break;
        default:
            v->u.b = FALSE_VAL;
            break;
    }
    v->type = value_type_bool;
}

value_t *value_transfer_string(value_t *v) {
    char *cs;
    char s[20];
    switch (v->type) {
        case value_type_double:
            sprintf(s, "%f", v->u.d);
            break;
        case value_type_integer:
            sprintf(s, "%i", v->u.i);
            break;
        case value_type_bool:
            sprintf(s, v->u.b == TRUE_VAL ? "true" : "false");
            break;
        case value_type_null:
            sprintf(s, "%s", "null");
            break;
        default:
            break;
    }
    if (is_not_empty(s)) {
        cs = (char *) memory_alloc(strlen(s) + 1);
        strcpy(cs, s);
        v->u.str.val = cs;
        v->u.str.len = strlen(s);
    }
    v->type = value_type_string;
}

/**
 *
 * @param v
 * @return
 */
value_t *value_duplicate(value_t *v, int is_sub) {
    value_t *dv;
    char *s;
    dv = (value_t *) memory_alloc(sizeof(value_t));
    dv->type = v->type;
    switch (v->type) {
        case value_type_bool:
            dv->u.b = v->u.b;
            break;
        case value_type_string:
            s = (char *) memory_alloc(v->u.str.len + 1);
            strcpy(s, v->u.str.val);
            dv->u.str.val = s;
            dv->u.str.len = v->u.str.len;
            break;
        case value_type_double:
            if (is_sub == 1) {
                dv->u.d = 0 - v->u.d;
            } else {
                dv->u.d = v->u.d;
            }
            break;
        case value_type_integer:
            if (is_sub == 1) {
                dv->u.i = 0 - v->u.i;
            } else {
                dv->u.i = v->u.i;
            }
            break;
        case value_type_null:
            dv->u.n = v->u.n;
            break;
    }
    dv->refer = dv->refer_count = 0;
    return dv;
}
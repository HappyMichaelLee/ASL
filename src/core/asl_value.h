#ifndef ASL_ASL_VALUE_H
#define ASL_ASL_VALUE_H

#define VALUE_TYPE_MAP_BODY(name, type) {STR(name),type}
#define VALUE_TYPE_MAP_DEF(name) value_type_map_t value_type_unit_##name
#define VALUE_TYPE_MAP_NAME(name) value_type_unit_##name
#define VALUE_TYPE_MAP_PNAME(name) &value_type_unit_##name

typedef struct value_s value_t;
typedef struct value_info_s value_info_t;
typedef struct value_list_s value_list_t;
typedef struct value_list_item_s value_list_item_t;
typedef struct value_type_map_s value_type_map_t;

struct value_info_s {
    hash_t *type_map_table;
    value_type_map_t **type_map;
};

enum value_type_e {
    value_type_integer = 1,
    value_type_double,
    value_type_string,
    value_type_bool,
    value_type_array,
    value_type_null,
    value_type_unknown
};

struct value_type_map_s {
    char *name;
    enum value_type_e type;
};

struct value_list_s {
    value_list_item_t *head;
    value_list_item_t *tail;
    int count;
};
struct value_list_item_s {
    value_list_item_t *next;
    value_t *v;
};


struct value_s {
    enum value_type_e type;
    int refer;
    int refer_count;
    union {
        char n;
        int i;
        char b;
        double d;
        hash_t *arr;
        struct {
            char *val;
            unsigned int len;
        } str;
    } u;
};

int value_init();

value_list_t *value_create_list();

value_list_t *value_insert_list(value_list_t *list, value_t *v);

int value_destroy_list(value_list_t *list);

value_t *value_create_null();

value_t *value_create_integer(int i);

value_t *value_create_string(char *s);

value_t *value_create_double(double d);

value_t *value_create_bool(char b);

int value_free(value_t *v);

int value_decr_quote(value_t *v);

int value_incr_quote(value_t *v);

value_t *value_transfer_int(value_t *v);

value_t *value_transfer_double(value_t *v);

value_t *value_transfer_bool(value_t *v);

value_t *value_transfer_string(value_t *v);

value_t *value_duplicate(value_t *v, int is_sub);

#endif

#ifndef ASL_ASL_COMPILER_H
#define ASL_ASL_COMPILER_H

/**
 *
 */
extern FILE *yyin;

extern int yyparse(void);

/**
 *
 */
typedef struct exp_s exp_t;
typedef struct func_exp_s func_exp_t;
typedef struct primary_exp_s primary_exp_t;
typedef struct binary_exp_s binary_exp_t;
typedef struct assign_exp_s assign_exp_t;
typedef struct exp_statement_s exp_statement_t;
typedef struct include_statement_s include_statement_t;
typedef struct for_statement_s for_statement_t;
typedef struct if_statement_s if_statement_t;
typedef struct assign_statement_s assign_statement_t;
typedef struct return_statement_s return_statement_t;
typedef struct global_statement_s global_statement_t;
typedef struct statement_s statement_t;
typedef struct statement_list_s statement_list_t;
typedef struct statement_list_item_s statement_list_item_t;
typedef struct func_s func_t;
typedef struct exp_list_s exp_list_t;
typedef struct exp_list_item_s exp_list_item_t;
typedef struct while_statement_s while_statement_t;
typedef struct func_param_s func_param_t;
typedef struct func_param_list_s func_param_list_t;
typedef struct func_param_list_item_s func_param_list_item_t;
typedef struct compiler_info_s compiler_info_t;
typedef struct compiler_input_list_s compiler_input_list_t;
typedef struct compiler_input_item_s compiler_input_item_t;

compiler_info_t global_compiler;

struct compiler_info_s {
    char *enter_filename;
    char *root_path;
    hash_t *include_file_table;
    hash_t *func_table;
    hash_t *global_table;
    hash_t *token_table;
    statement_list_t *statement_list;
    compiler_input_list_t *input_list;
    compiler_input_item_t *input_item;
};

struct compiler_input_list_s {
    compiler_input_item_t *head;
    compiler_input_item_t *tail;
    int count;
};
struct compiler_input_item_s {
    FILE *input;
    char *filename;
    int scan_line;
    compiler_input_item_t *next;
    compiler_input_item_t *prev;
};
enum exp_type_e {
    exp_type_primary = 1,
    exp_type_binary,
    exp_type_assign,
    exp_type_global,
    exp_type_func_call
};
struct exp_list_s {
    exp_list_item_t *head;
    exp_list_item_t *tail;
    int count;
};
struct exp_list_item_s {
    exp_t *e;
    exp_list_item_t *next;
};
struct exp_s {
    enum exp_type_e type;
    union {
        primary_exp_t *p;
        binary_exp_t *b;
        assign_exp_t *a;
        func_exp_t *f;
    } u;
};

struct func_exp_s {
    char *func_name;
    int func_name_len;
    int is_sub;
    exp_list_t *params;
};
enum exp_value_type_e {
    exp_value_type_string = 1,
    exp_value_type_integer,
    exp_value_type_double,
    exp_value_type_var,
    exp_value_type_bool,
    exp_value_type_null
};
enum exp_value_defined_e {
    exp_value_defined_null = 1,
    exp_value_defined_true,
    exp_value_defined_false
};
struct primary_exp_s {
    enum exp_value_type_e type;
    int is_sub;
    union {
        int i;
        double d;
        char b;
        char n;
        struct {
            char *val;
            int len;
        } str;
    } u;
};

enum binary_type_e {
    binary_type_plus = 1,
    binary_type_sub,
    binary_type_mul,
    binary_type_div,
    binary_type_gt,
    binary_type_ge,
    binary_type_lt,
    binary_type_le,
    binary_type_eq,
    binary_type_neq
};

struct binary_exp_s {
    enum binary_type_e type;
    exp_t *l;
    exp_t *r;
};

struct assign_exp_s {
    char *var;
    int var_len;
    exp_t *e;
};


/**
 * 语句
 */
enum statement_type_e {
    statement_type_exp = 1,
    statement_type_if,
    statement_type_for,
    statement_type_break,
    statement_type_continue,
    statement_type_return,
    statement_type_global,
    statement_type_include,
    statement_type_while,
    statement_type_assign
};

struct exp_statement_s {
    exp_t *e;
};

struct include_statement_s {
    exp_t *e;
};
struct for_statement_s {
    exp_t *b;
    exp_t *c;
    exp_t *a;
    statement_list_t *sl;
};
struct while_statement_s {
    exp_t *c;
    statement_list_t *sl;
    int is_do;
};
struct if_statement_s {
    exp_t *c;
    statement_list_t *isl;
    statement_list_t *esl;
};
struct return_statement_s {
    exp_t *e;
};
struct global_statement_s {
    char *identifier;
    exp_t *e;
};
struct assign_statement_s {
    char *identifier;
    exp_t *e;
};
struct statement_s {
    enum statement_type_e type;
    union {
        global_statement_t *g;
        exp_statement_t *e;
        for_statement_t *f;
        while_statement_t *w;
        if_statement_t *i;
        return_statement_t *r;
        include_statement_t *in;
        assign_statement_t *a;
    } u;
};
struct statement_list_s {
    statement_list_item_t *head;
    statement_list_item_t *tail;
    int count;
};
struct statement_list_item_s {
    statement_list_item_t *next;
    statement_t *s;
};
enum func_type_e {
    func_type_native = 1,
    func_type_user
};

struct func_s {
    char *name;
    enum func_type_e type;
    statement_list_t *sl;
    func_param_list_t *params;

    value_t *(*func_addr)(hash_t *var_table);
};

struct func_param_list_s {
    func_param_list_item_t *head;
    func_param_list_item_t *tail;
    int count;
};
struct func_param_list_item_s {
    func_param_list_item_t *next;
    func_param_t *p;
};

struct func_param_s {
    char *name;
    int name_len;
    enum value_type_e force_type;
    int is_required;
    void *default_val;
};


/**
 * init
 */

void compiler_init();

void compiler_run();

void compiler_load_input(char *filename, FILE *input);

void compiler_unload_input();

void compiler_shutdown();

void compiler_insert_global_statement_list(statement_t *s);

char *compiler_find_token(char *s, int len);

int compiler_insert_token(char *s, int len);

/**
 * statement
 */

statement_t *compiler_create_include_statement(exp_t *e);

statement_t *compiler_create_exp_statement(exp_t *e);

statement_t *compiler_create_if_statement(exp_t *c, statement_list_t *isl, statement_list_t *esl);

statement_t *compiler_create_for_statement(exp_t *b, exp_t *c, exp_t *a, statement_list_t *sl);

statement_t *compiler_create_while_statement(exp_t *c, statement_list_t *sl, int is_do);

statement_t *compiler_create_return_statement(exp_t *e);

statement_t *compiler_create_break_statement();

statement_t *compiler_create_continue_statement();

statement_list_t *compiler_create_statement_list(statement_t *s);

statement_list_t *compiler_insert_statement_list(statement_list_t *sl, statement_t *s);

/**
 * expression
 */
exp_t *compiler_create_integer_exp(int i, int is_sub);

exp_t *compiler_create_double_exp(double d, int is_sub);

exp_t *compiler_create_string_exp(char *s);

exp_t *compiler_create_var_exp(char *var, int is_sub);

exp_t *compiler_create_binary_exp(enum binary_type_e bt, exp_t *l, exp_t *r);

exp_t *compiler_create_assign_exp(char *var, exp_t *e);

exp_t *compiler_create_call_func_exp(char *func_name, exp_list_t *params, int is_sub);

exp_t *compiler_create_value_exp(enum exp_value_defined_e defined);

exp_list_t *compiler_create_exp_list(exp_t *e);

exp_list_t *compiler_insert_exp_list(exp_list_t *el, exp_t *e);


/**
 * function
 */
func_t *compiler_declare_func(char *name, func_param_list_t *params, statement_list_t *sl);

func_param_list_t *compiler_create_param_list();

func_param_list_t *compiler_insert_param_list(func_param_list_t *fpl, func_param_t *fp);

func_param_t *compiler_create_param(char *name, char *type, exp_t *e);

#endif

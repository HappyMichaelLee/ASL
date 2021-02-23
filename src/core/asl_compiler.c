#include <asl_core.h>
#include <asl_compiler.h>

extern value_info_t global_value;

/**
 * 初始化编译器
 * @param root_filename
 */
void compiler_init() {
    compiler_input_list_t *input_list;

    global_compiler.func_table = hash_create();
    global_compiler.global_table = hash_create();
    global_compiler.include_file_table = hash_create();
    global_compiler.token_table = hash_create();
    global_compiler.root_path = util_get_cwd();
    global_compiler.statement_list = (statement_list_t *) memory_alloc(sizeof(statement_list_t));
    global_compiler.statement_list->head = global_compiler.statement_list->tail = NULL;
    global_compiler.statement_list->count = 0;

    input_list = (compiler_input_list_t *) memory_alloc(sizeof(compiler_input_list_t));
    input_list->head = input_list->tail = NULL;
    input_list->count = 0;
    global_compiler.input_list = input_list;
    global_compiler.input_item = NULL;
}

/**
 *
 * @param s
 * @param len
 * @return
 */
char *compiler_find_token(char *s, int len) {
    char *str;
    str = (char *) hash_find(global_compiler.token_table, s, len);
    return str;
}

/**
 *
 * @param s
 * @param len
 * @return
 */
int compiler_insert_token(char *s, int len) {
    char *exit;
    exit = compiler_find_token(s, len);
    if (is_not_empty(exit)) {
        return RET_FAILURE;
    }
    return hash_insert_or_update(global_compiler.token_table, s, len, s);
}

/**
 * 加载输入
 * @param filename
 * @param input
 */
void compiler_load_input(char *filename, FILE *input) {
    char *real_filename;
    compiler_input_item_t *input_item, *tail_item;
    real_filename = util_get_realpath(filename);

    input_item = (compiler_input_item_t *) memory_alloc((sizeof(compiler_input_item_t)));
    input_item->input = input;
    input_item->filename = real_filename;
    input_item->scan_line = 0;
    input_item->next = input_item->prev = NULL;
    if (global_compiler.input_list->count == 0) {
        global_compiler.input_item = input_item;
        global_compiler.input_list->head = global_compiler.input_list->tail = input_item;
        global_compiler.enter_filename = real_filename;
    } else {
        tail_item = global_compiler.input_list->tail;
        tail_item->next = input_item;
        input_item->prev = tail_item;
        global_compiler.input_list->tail = input_item;
    }
    global_compiler.input_list->count++;
    yyin = input;
    hash_insert_or_update(global_compiler.include_file_table, real_filename, strlen(real_filename), real_filename);
}

/**
 * 退载输入
 */
void compiler_unload_input() {
    compiler_input_item_t *input_item;
    input_item = global_compiler.input_item;
    if (global_compiler.input_list->count <= 1) {
        global_compiler.input_item = NULL;
        global_compiler.input_list->head = NULL;
        yyin = NULL;
    } else {
        global_compiler.input_item = input_item->prev;
        global_compiler.input_item->next = NULL;
        yyin = global_compiler.input_item->input;
    }
    global_compiler.input_list->tail = global_compiler.input_item;
    global_compiler.input_list->count--;
    fclose(input_item->input);
    memory_free(input_item);
}

/**
 * 编译器运行
 */
void compiler_run() {
    if (yyparse()) {
        exception_fatal_exc("System parse failed!");
    }
}

/**
 * 编译器关闭
 */
void compiler_shutdown() {
    memory_free(global_compiler.input_list);
    hash_destroy(global_compiler.global_table);
    hash_destroy(global_compiler.include_file_table);
    hash_destroy(global_compiler.func_table);
}

/**
 * 插入全局语句链表
 * @param s
 */
void compiler_insert_global_statement_list(statement_t *s) {
    compiler_insert_statement_list(global_compiler.statement_list, s);
}

/**
 * 定义函数
 * @param name
 * @param params
 * @param sl
 * @return
 */
func_t *compiler_declare_func(char *name, func_param_list_t *params, statement_list_t *sl) {
    void *exist;
    func_t *f;
    exist = hash_find(global_compiler.func_table, name, strlen(name));
    if (is_not_empty(exist)) {
        exception_fatal_compiler_exec("Function:%s has been declared.", name);
    }
    f = (func_t *) memory_alloc(sizeof(func_t));
    f->name = name;
    f->params = params;
    f->sl = sl;
    f->type = func_type_user;
    f->func_addr = NULL;
    hash_insert_or_update(global_compiler.func_table, name, strlen(name), (void *) f);
}

/**
 * 创建参数
 * @param name
 * @param type
 * @param e
 * @return
 */
func_param_t *compiler_create_param(char *name, char *type, exp_t *e) {
    void *data;
    func_param_t *p;
    p = (func_param_t *) memory_alloc(sizeof(func_param_t));
    p->name = name;
    p->name_len = strlen(name);
    data = hash_find(global_value.type_map_table, type, strlen(type));
    if (is_empty(data)) {
        exception_fatal_compiler_exec("force type %s is not defined", type);
    }
    p->force_type = (enum value_type_e) data;
    p->default_val = (void *) e;
    return p;
}

/**
 * 创建参数链表
 * @param p
 * @return
 */
func_param_list_t *compiler_create_param_list() {
    func_param_list_t *list;
    list = (func_param_list_t *) memory_alloc(sizeof(func_param_list_t));
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
func_param_list_t *compiler_insert_param_list(func_param_list_t *list, func_param_t *p) {
    func_param_list_item_t *item;
    item = (func_param_list_item_t *) memory_alloc(sizeof(func_param_list_item_t));
    item->p = p;
    item->next = NULL;
    if (list->count == 0) {
        list->head = list->tail = item;
    } else {
        list->tail->next = item;
        list->tail = item;
    }
    list->count++;
    return list;
}

/**
 * 创建include语句
 * @param e
 * @return
 */
statement_t *compiler_create_include_statement(exp_t *e) {
    include_statement_t *is;
    statement_t *s;
    is = (include_statement_t *) memory_alloc(sizeof(include_statement_t));
    is->e = e;
    s = (statement_t *) memory_alloc(sizeof(statement_t));
    s->type = statement_type_include;
    s->u.in = is;
    return s;
}

/**
 * 创建表达式语句
 * @param e
 * @return
 */
statement_t *compiler_create_exp_statement(exp_t *e) {
    exp_statement_t *es;
    statement_t *s;
    es = (exp_statement_t *) memory_alloc(sizeof(exp_statement_t));
    es->e = e;
    s = (statement_t *) memory_alloc(sizeof(statement_t));
    s->type = statement_type_exp;
    s->u.e = es;
    return s;
}

/**
 * 创建if语句
 * @param c
 * @param isl
 * @param esl
 * @return
 */

statement_t *compiler_create_if_statement(exp_t *c, statement_list_t *isl, statement_list_t *esl) {
    if_statement_t *ifs;
    statement_t *s;
    ifs = (if_statement_t *) memory_alloc(sizeof(if_statement_t));
    s = (statement_t *) memory_alloc(sizeof(statement_t));
    s->type = statement_type_if;
    ifs->c = c;
    ifs->isl = isl;
    ifs->esl = esl;
    s->u.i = ifs;
    return s;
}

/**
 * 创建for语句
 * @param b
 * @param c
 * @param a
 * @param sl
 * @return
 */
statement_t *compiler_create_for_statement(exp_t *b, exp_t *c, exp_t *a, statement_list_t *sl) {
    for_statement_t *fs;
    statement_t *s;
    fs = (for_statement_t *) memory_alloc(sizeof(for_statement_t));
    s = (statement_t *) memory_alloc(sizeof(statement_t));
    s->type = statement_type_for;
    fs->c = c;
    fs->a = a;
    fs->b = b;
    fs->sl = sl;
    s->u.f = fs;
    return s;
}

/**
 * 创建while语句
 * @param c
 * @param sl
 * @param is_do
 * @return
 */
statement_t *compiler_create_while_statement(exp_t *c, statement_list_t *sl, int is_do) {
    while_statement_t *ws;
    statement_t *s;
    ws = (while_statement_t *) memory_alloc(sizeof(while_statement_t));
    s = (statement_t *) memory_alloc(sizeof(statement_t));
    s->type = statement_type_while;
    ws->c = c;
    ws->sl = sl;
    ws->is_do = is_do;
    s->u.w = ws;
    return s;
}

/**
 * 创建跳出语句
 * @return
 */
statement_t *compiler_create_break_statement() {
    statement_t *s;
    s = (statement_t *) memory_alloc(sizeof(statement_t));
    s->type = statement_type_break;
    return s;
}

/**
 * 创建继续语句
 * @return
 */
statement_t *compiler_create_continue_statement() {
    statement_t *s;
    s = (statement_t *) memory_alloc(sizeof(statement_t));
    s->type = statement_type_continue;
    return s;
}

/**
 * 创建返回语句
 * @param e
 * @return
 */
statement_t *compiler_create_return_statement(exp_t *e) {
    statement_t *s;
    return_statement_t *rs;
    rs = (return_statement_t *) memory_alloc(sizeof(return_statement_t));
    s = (statement_t *) memory_alloc(sizeof(statement_t));
    s->type = statement_type_return;
    rs->e = e;
    s->u.r = rs;
    return s;
}


/**
 * 创建语句链表
 * @param s
 * @return
 */
statement_list_t *compiler_create_statement_list(statement_t *s) {
    statement_list_t *list;
    statement_list_item_t *item;
    list = (statement_list_t *) memory_alloc(sizeof(statement_list_t));
    item = (statement_list_item_t *) memory_alloc(sizeof(statement_list_item_t));
    item->s = s;
    item->next = NULL;
    list->head = list->tail = item;
    list->count = 1;
    return list;
}

/**
 * 插入语句链表
 * @param list
 * @param s
 * @return
 */
statement_list_t *compiler_insert_statement_list(statement_list_t *list, statement_t *s) {
    statement_list_item_t *item;
    item = (statement_list_item_t *) memory_alloc(sizeof(statement_list_item_t));
    if (list->count == 0) {
        item->s = s;
        item->next = NULL;
        list->head = list->tail = item;
        list->count = 1;
    } else {
        item->s = s;
        item->next = NULL;
        list->tail->next = item;
        list->count++;
        list->tail = item;
    }
    return list;
}

/**
 * 创建赋值表达式
 * @param variable
 * @param e
 * @return
 */
exp_t *compiler_create_assign_exp(char *var, exp_t *e) {
    assign_exp_t *ae;
    exp_t *te;
    ae = (assign_exp_t *) memory_alloc(sizeof(assign_exp_t));
    te = (exp_t *) memory_alloc(sizeof(exp_t));
    ae->e = e;
    ae->var = var;
    ae->var_len = strlen(var);
    te->u.a = ae;
    te->type = exp_type_assign;
    return te;
}

/**
 *
 * @param defined
 * @return
 */
exp_t *compiler_create_value_exp(enum exp_value_defined_e defined) {
    primary_exp_t *pe;
    exp_t *te;
    pe = (primary_exp_t *) memory_alloc(sizeof(primary_exp_t));
    te = (exp_t *) memory_alloc(sizeof(exp_t));
    switch (defined) {
        case exp_value_defined_false:
            pe->type = exp_value_type_bool;
            pe->u.b = FALSE_VAL;
            break;
        case exp_value_defined_null:
            pe->type = exp_value_type_null;
            break;
        case exp_value_defined_true:
            pe->type = exp_value_type_bool;
            pe->u.b = TRUE_VAL;
            break;
    }
    te->type = exp_type_primary;
    te->u.p = pe;
    return te;
}

/**
 * 创建数字表达式
 * @param i
 * @return
 */
exp_t *compiler_create_integer_exp(int i, int is_sub) {
    primary_exp_t *pe;
    exp_t *te;
    pe = (primary_exp_t *) memory_alloc(sizeof(primary_exp_t));
    te = (exp_t *) memory_alloc(sizeof(exp_t));
    pe->is_sub = 0;
    pe->type = exp_value_type_integer;
    pe->u.i = i;
    pe->is_sub = is_sub;
    te->type = exp_type_primary;
    te->u.p = pe;
    return te;
}

/**
 * 创建标示表达式
 * @param variable
 * @return
 */
exp_t *compiler_create_var_exp(char *var, int is_sub) {
    primary_exp_t *pe;
    exp_t *te;
    pe = (primary_exp_t *) memory_alloc(sizeof(primary_exp_t));
    te = (exp_t *) memory_alloc(sizeof(exp_t));
    pe->is_sub = is_sub;
    pe->type = exp_value_type_var;
    pe->u.str.val = var;
    pe->u.str.len = strlen(var);
    te->type = exp_type_primary;
    te->u.p = pe;
    return te;
}

/**
 * 创建双精度表达式
 * @param d
 * @return
 */
exp_t *compiler_create_double_exp(double d, int is_sub) {
    primary_exp_t *pe;
    exp_t *te;
    pe = (primary_exp_t *) memory_alloc(sizeof(primary_exp_t));
    te = (exp_t *) memory_alloc(sizeof(exp_t));
    pe->is_sub = 0;
    pe->type = exp_value_type_double;
    pe->u.d = d;
    pe->is_sub = is_sub;
    te->type = exp_type_primary;
    te->u.p = pe;
    return te;
}

/**
 * 创建字符串表达式
 * @param s
 * @return
 */
exp_t *compiler_create_string_exp(char *s) {
    primary_exp_t *pe;
    exp_t *te;
    pe = (primary_exp_t *) memory_alloc(sizeof(primary_exp_t));
    te = (exp_t *) memory_alloc(sizeof(exp_t));
    pe->is_sub = 0;
    pe->type = exp_value_type_string;
    pe->u.str.val = s;
    pe->u.str.len = strlen(s);
    te->type = exp_type_primary;
    te->u.p = pe;
    return te;
}

/**
 * 创建二元表达式
 * @param type
 * @param l
 * @param r
 * @return
 */
exp_t *compiler_create_binary_exp(enum binary_type_e type, exp_t *l, exp_t *r) {
    binary_exp_t *be;
    exp_t *te;
    be = (binary_exp_t *) memory_alloc(sizeof(binary_exp_t));
    te = (exp_t *) memory_alloc(sizeof(exp_t));
    be->l = l;
    be->r = r;
    be->type = type;
    te->type = exp_type_binary;
    te->u.b = be;
    return te;
}

/**
 * 创建调用函数表达式
 * @param func_name
 * @param params
 * @return
 */
exp_t *compiler_create_call_func_exp(char *func_name, exp_list_t *params, int is_sub) {
    func_exp_t *fe;
    exp_t *te;
    fe = (func_exp_t *) memory_alloc(sizeof(func_exp_t));
    te = (exp_t *) memory_alloc(sizeof(exp_t));
    fe->func_name = func_name;
    fe->func_name_len = strlen(func_name);
    fe->params = params;
    fe->is_sub = is_sub;
    te->type = exp_type_func_call;
    te->u.f = fe;
    return te;
}

/**
 * 创建表达式链表
 * @param e
 * @return
 */
exp_list_t *compiler_create_exp_list(exp_t *e) {
    exp_list_t *list;
    exp_list_item_t *item;
    list = (exp_list_t *) memory_alloc(sizeof(exp_list_t));
    item = (exp_list_item_t *) memory_alloc(sizeof(exp_list_item_t));
    item->e = e;
    item->next = NULL;
    list->head = list->tail = item;
    list->count = 1;
    return list;
}

/**
 * 插入表达式链表
 * @param list
 * @param e
 * @return
 */
exp_list_t *compiler_insert_exp_list(exp_list_t *list, exp_t *e) {
    exp_list_item_t *item;
    item = (exp_list_item_t *) memory_alloc(sizeof(exp_list_item_t));
    item->e = e;
    item->next = NULL;
    list->tail->next = item;
    list->count++;
    list->tail = item;
    return list;
}
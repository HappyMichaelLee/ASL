#ifndef ASL_ASL_EXECUTE_H
#define ASL_ASL_EXECUTE_H

#define EXR_FIND_VAR(var_table, var, var_len) (value_t *) hash_find(var_table ,var ,var_len)

typedef struct executor_info_s executor_info_t;
typedef struct executor_control_s executor_control_t;

struct executor_info_s {
    hash_t *func_table;
    hash_t *module_table;
    int module_count;
    hash_t *var_table;
    statement_list_t *statement_list;

};
enum executor_control_type_e {
    executor_control_type_break = 1,
    executor_control_type_continue,
    executor_control_type_return,
    executor_control_type_default
};

struct executor_control_s {
    enum executor_control_type_e type;
    value_t *result;
};

void executor_init();

void executor_run();

executor_control_t *executor_run_statement(statement_list_t *sl, hash_t *var_table);

executor_control_t *executor_create_control(enum executor_control_type_e type);

void executor_shutdown();

#endif

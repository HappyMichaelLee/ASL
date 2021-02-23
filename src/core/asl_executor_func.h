#ifndef ASL_ASL_EXECUTOR_FUNC_H
#define ASL_ASL_EXECUTOR_FUNC_H

value_t *executor_func_run(func_exp_t *fe, hash_t *var_table);

value_t *executor_func_native_call(func_t *f, hash_t *var_table);

value_t *executor_func_user_call(func_t *f, hash_t *var_table);

#endif

#ifndef ASL_ASL_EXECUTOR_EXP_H
#define ASL_ASL_EXECUTOR_EXP_H

value_t *executor_exp_run(exp_t *e, hash_t *var_table);

value_t *executor_exp_run_primary(primary_exp_t *pe, hash_t *var_table);

value_t *executor_exp_run_assign(assign_exp_t *ae, hash_t *var_table);

value_t *executor_exp_run_binary(binary_exp_t *be, hash_t *var_table);

#endif

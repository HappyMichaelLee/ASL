#ifndef ASL_ASL_EXECUTOR_LOOP_H
#define ASL_ASL_EXECUTOR_LOOP_H

executor_control_t *executor_while_run(while_statement_t *ws, hash_t *var_table);

executor_control_t *executor_if_run(if_statement_t *is, hash_t *var_table);

executor_control_t *executor_for_run(for_statement_t *fs, hash_t *var_table);

#endif

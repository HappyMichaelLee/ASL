#ifndef ASL_ASL_EXECUTOR_CONTROL_H
#define ASL_ASL_EXECUTOR_CONTROL_H

executor_control_t *executor_return_run(return_statement_t *rs, hash_t *var_table);

executor_control_t *executor_continue_run();

executor_control_t *executor_break_run();

#endif

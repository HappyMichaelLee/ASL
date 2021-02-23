#ifndef ASL_ASL_EXECUTOR_BIN_H
#define ASL_ASL_EXECUTOR_BIN_H

value_t *executor_bin_plus(value_t *left, value_t *right);

value_t *executor_bin_sub(value_t *left, value_t *right);

value_t *executor_bin_mul(value_t *left, value_t *right);

value_t *executor_bin_div(value_t *left, value_t *right);

value_t *executor_bin_gt(value_t *left, value_t *right);

value_t *executor_bin_ge(value_t *left, value_t *right);

value_t *executor_bin_lt(value_t *left, value_t *right);

value_t *executor_bin_le(value_t *left, value_t *right);

value_t *executor_bin_ne(value_t *left, value_t *right);

value_t *executor_bin_eq(value_t *left, value_t *right);

value_t *executor_bin_neq(value_t *left, value_t *right);

void executor_bin_operate(value_t *left, value_t *right, int *left_i, int *right_i, double *left_d,
                          double *right_d, int *left_value_type, int *right_value_type);

#endif

#include <asl_core.h>
#include <asl_executor.h>
#include <asl_executor_exp.h>
#include <asl_executor_control.h>

executor_control_t *executor_return_run(return_statement_t *rs, hash_t *var_table) {
    executor_control_t *ec;
    ec = executor_create_control(executor_control_type_return);
    if (is_not_empty(rs->e)) {
        ec->result = executor_exp_run(rs->e, var_table);
    } else {
        ec->result = value_create_null();
    }
    return ec;
}

/**
 *
 * @return
 */
executor_control_t *executor_continue_run() {
    executor_control_t *ec;
    ec = executor_create_control(executor_control_type_continue);
    ec->result = value_create_null();
    return ec;
}

/**
 *
 * @return
 */
executor_control_t *executor_break_run() {
    executor_control_t *ec;
    ec = executor_create_control(executor_control_type_break);
    ec->result = value_create_null();
    return ec;
}
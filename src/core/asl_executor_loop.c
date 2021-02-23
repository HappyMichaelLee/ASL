#include <asl_core.h>
#include <asl_executor.h>
#include <asl_executor_exp.h>
#include <asl_executor_loop.h>

executor_control_t *executor_while_run(while_statement_t *ws, hash_t *var_table) {
    value_t *cv, *tv;
    executor_control_t *ec;
    char condition;
    if (ws->is_do == 1) {
        condition = TRUE_VAL;
    } else {
        cv = executor_exp_run(ws->c, var_table);
        tv = value_transfer_bool(cv);
        condition = tv->u.b;
        if (tv->refer_count == 0) {
            value_free(tv);
        }
    }
    while (condition == TRUE_VAL) {
        ec = executor_run_statement(ws->sl, var_table);
        if (is_not_empty(ec)) {
            if (ec->type == executor_control_type_return) {
                break;
            } else if (ec->type == executor_control_type_continue) {
                memory_free(ec);
                ec = NULL;
                continue;
            } else if (ec->type == executor_control_type_break) {
                memory_free(ec);
                ec = NULL;
                break;
            }
        }
        cv = executor_exp_run(ws->c, var_table);
        tv = value_transfer_bool(cv);
        condition = tv->u.b;
        if (tv->refer_count == 0) {
            value_free(tv);
        }
    }
    return ec;
}

/**
 *
 * @param is
 * @param var_table
 * @return
 */
executor_control_t *executor_if_run(if_statement_t *is, hash_t *var_table) {
    value_t *cv, *tv;
    executor_control_t *ec;
    cv = executor_exp_run(is->c, var_table);
    tv = value_transfer_bool(cv);
    if (tv->u.b == TRUE_VAL) {
        ec = executor_run_statement(is->isl, var_table);
    } else {
        ec = executor_run_statement(is->esl, var_table);
    }
    if (tv->refer_count == 0) {
        value_free(tv);
    }
    if (is_not_empty(ec)) {
    }
    return ec;
}

/**
 *
 * @param fs
 * @param var_table
 * @return
 */
executor_control_t *executor_for_run(for_statement_t *fs, hash_t *var_table) {
    value_t *cv, *tv, *bv, *av;
    executor_control_t *ec;
    bv = executor_exp_run(fs->b, var_table);
    if (bv->refer_count == 0) {
        value_free(bv);
    }
    cv = executor_exp_run(fs->c, var_table);
    tv = value_transfer_bool(cv);
    ec = NULL;
    while (tv->u.b == TRUE_VAL) {
        if (tv->refer_count == 0) {
            value_free(tv);
        }
        av = executor_exp_run(fs->a, var_table);
        if (av->refer_count == 0) {
            value_free(av);
        }
        cv = executor_exp_run(fs->c, var_table);
        tv = value_transfer_bool(cv);
        ec = executor_run_statement(fs->sl, var_table);
        if (is_not_empty(ec)) {
            if (ec->type == executor_control_type_return) {
                break;
            } else if (ec->type == executor_control_type_continue) {
                memory_free(ec);
                ec = NULL;
                continue;
            } else if (ec->type == executor_control_type_break) {
                memory_free(ec);
                ec = NULL;
                break;
            }
        }
    }
    return ec;
}
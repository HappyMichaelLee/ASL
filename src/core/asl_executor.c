#include <asl_core.h>
#include <asl_executor.h>
#include <asl_executor_exp.h>
#include <asl_executor_control.h>
#include <asl_executor_loop.h>

executor_info_t global_executor;

/**
 *
 */
void executor_init() {
    global_executor.module_table = hash_create();
    global_executor.module_count = 0;
    global_executor.func_table = global_compiler.func_table;
    global_executor.statement_list = global_compiler.statement_list;
    global_executor.var_table = global_compiler.global_table;
}

/**
 *
 */
void executor_run() {
    executor_control_t *ec;
    ec = executor_run_statement(global_executor.statement_list, global_executor.var_table);
    if (is_not_empty(ec)) {
        if (is_not_empty(ec->result)) {
            output_debug("value_free call 3");
            value_free(ec->result);
        }
        memory_free(ec);
    }
}

/**
 *
 * @param sl
 * @param var_table
 * @return
 */
executor_control_t *executor_run_statement(statement_list_t *sl, hash_t *var_table) {
    statement_list_item_t *item;
    executor_control_t *ec;
    value_t *v;
    statement_t *s;
    int need_jump = 0;
    if (is_empty(sl)) {
        return NULL;
    }
    item = sl->head;
    while (item) {
        s = item->s;
        v = NULL;
        ec = NULL;
        switch (s->type) {
            case statement_type_include:
                output_debug("statement_type_include");
                break;
            case statement_type_return:
                ec = executor_return_run(s->u.r, var_table);
                break;
            case statement_type_continue:
                ec = executor_continue_run();
                break;
            case statement_type_break:
                ec = executor_break_run();
                break;
            case statement_type_while:
                ec = executor_while_run(s->u.w, var_table);
                break;
            case statement_type_for:
                ec = executor_for_run(s->u.f, var_table);
                break;
            case statement_type_if:
                ec = executor_if_run(s->u.i, var_table);
                break;
            case statement_type_exp:
                v = executor_exp_run(s->u.e->e, var_table);
                ec = NULL;
                break;
            case statement_type_global:
                output_debug("statement_type_global");
                break;
            default:
                output_debug("statement_type_assign");
        }
        item = item->next;
        /**
         * ec is not empty
         */
        if (is_not_empty(ec)) {
            switch (ec->type) {
                case executor_control_type_break:
                case executor_control_type_continue:
                case executor_control_type_return:
                    need_jump = 1;
                    break;
                case executor_control_type_default:
                default:
                    need_jump = 0;
                    break;
            }
            if (is_not_empty(item) && need_jump != 1) {
                if (is_not_empty(ec->result) && ec->result->refer_count == 0) {
                    value_free(ec->result);
                }
                memory_free(ec);
                ec = NULL;
            }
        } else {
            if (is_not_empty(item) && is_not_empty(v) && v->refer_count == 0) {
                value_free(v);
            }
        }
        /**
         * jump out
         */
        if (need_jump == 1) {
            break;
        }
    }
    return ec;
}

/**
 *
 * @param type
 * @return
 */
executor_control_t *executor_create_control(enum executor_control_type_e type) {
    executor_control_t *ec;
    ec = (executor_control_t *) memory_alloc(sizeof(executor_control_t));
    ec->type = type;
    ec->result = NULL;
    return ec;
}

void executor_shutdown() {

}
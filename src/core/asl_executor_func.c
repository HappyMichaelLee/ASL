#include <asl_core.h>
#include <asl_executor_exp.h>
#include <asl_executor_func.h>

extern executor_info_t global_executor;

/**
 *
 * @param fe
 * @param var_table
 * @return
 */
value_t *executor_func_run(func_exp_t *fe, hash_t *var_table) {
    func_t *f;
    func_param_list_item_t *fpli;
    exp_list_item_t *eli;
    value_list_t *vl;
    value_list_item_t *vli;
    value_t *v, *dv, *ev, *ddv;
    hash_t *func_var_table;
    hash_bucket_t *hb;
    f = (func_t *) hash_find(global_executor.func_table, fe->func_name, fe->func_name_len);
    if (is_empty(f)) {
        exception_fatal_exc("Function %s is not declared!", fe->func_name);
    }
    if (is_not_empty(fe->params)) {
        eli = fe->params->head;
    } else {
        eli = NULL;
    }
    vl = value_create_list();
    while (eli) {
        v = executor_exp_run(eli->e, var_table);
        value_insert_list(vl, v);
        eli = eli->next;
    }
    if (is_empty(f->params) || f->params->count == 0) {
        value_destroy_list(vl);
    }
    if (is_not_empty(f->params)) {
        fpli = f->params->head;
    } else {
        fpli = NULL;
    }
    func_var_table = hash_create();
    vli = vl->head;
    while (fpli) {
        /**
         * not pass parameter
         */
        if (is_empty(vli)) {
            if (is_not_empty(fpli->p->default_val)) {
                dv = executor_exp_run((exp_t *) fpli->p->default_val, var_table);
            } else {
                dv = NULL;
            }
        } else {
            dv = vli->v;
        }
        switch (fpli->p->force_type) {
            case value_type_integer:
                value_transfer_int(dv);
                break;
            case value_type_double:
                value_transfer_double(dv);
                break;
            case value_type_string:
                value_transfer_string(dv);
                break;
            case value_type_bool:
                value_transfer_bool(dv);
                break;
            default:
                break;
        }
        if (is_empty(dv)) {
            exception_fatal_exc("On execute function %s,please pass parameter %s!", f->name, fpli->p->name);
        }
        ddv = value_duplicate(dv, 0);
        if (dv->refer_count == 0) {
            value_free(dv);
        }
        hash_insert_or_update(func_var_table, fpli->p->name, fpli->p->name_len, (void *) ddv);
        if (is_not_empty(vli)) {
            vli = vli->next;
        }
        fpli = fpli->next;
    }
    switch (f->type) {
        case func_type_native:
            ev = executor_func_native_call(f, func_var_table);
            break;
        case func_type_user:
        default:
            ev = executor_func_user_call(f, func_var_table);
            break;
    }

    hb = func_var_table->list_head;
    if (is_not_empty(hb)) {
        while (hb) {
            if (is_not_empty(hb->data)) {
                memory_free(hb->data);
            }
            hb = hb->list_next;
        }
    }
    hash_destroy(func_var_table);
    return ev;
}

/**
 *
 * @param f
 * @param var_table
 * @return
 */
value_t *executor_func_native_call(func_t *f, hash_t *var_table) {
    return f->func_addr(var_table);
}

/**
 *
 * @param f
 * @param var_table
 * @return
 */
value_t *executor_func_user_call(func_t *f, hash_t *var_table) {
    value_t *v;
    executor_control_t *ec;
    ec = executor_run_statement(f->sl, var_table);
    v = ec->result;
    if (is_empty(v)) {
        v = value_create_null();
    }
    return v;
}
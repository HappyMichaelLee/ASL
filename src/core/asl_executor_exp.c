#include <asl_core.h>
#include <asl_executor_exp.h>
#include <asl_executor_bin.h>
#include <asl_executor_func.h>

/**
 *
 * @param e
 * @param var_table
 * @return
 */
value_t *executor_exp_run(exp_t *e, hash_t *var_table) {
    switch (e->type) {
        case exp_type_assign:
            return executor_exp_run_assign(e->u.a, var_table);
        case exp_type_binary:
            return executor_exp_run_binary(e->u.b, var_table);
        case exp_type_func_call:
            return executor_func_run(e->u.f, var_table);
        case exp_type_global:
            break;
        case exp_type_primary:
            return executor_exp_run_primary(e->u.p, var_table);
        default:
            exception_fatal_exc("Unknown expression type %d", e->type);
    }
    return NULL;
}

/**
 *
 * @param be
 * @param var_table
 * @return
 */
value_t *executor_exp_run_binary(binary_exp_t *be, hash_t *var_table) {
    value_t *left, *right, *result;
    left = executor_exp_run(be->l, var_table);
    right = executor_exp_run(be->r, var_table);
    switch (be->type) {
        case binary_type_div:
            result = executor_bin_div(left, right);
            break;
        case binary_type_eq:
            result = executor_bin_eq(left, right);
            break;
        case binary_type_ge:
            result = executor_bin_ge(left, right);
            break;
        case binary_type_gt:
            result = executor_bin_gt(left, right);
            break;
        case binary_type_le:
            result = executor_bin_le(left, right);
            break;
        case binary_type_lt:
            result = executor_bin_lt(left, right);
            break;
        case binary_type_mul:
            result = executor_bin_mul(left, right);
            break;
        case binary_type_neq:
            result = executor_bin_neq(left, right);
            break;
        case binary_type_plus:
            result = executor_bin_plus(left, right);
            break;
        case binary_type_sub:
            result = executor_bin_sub(left, right);
            break;
        default:
            exception_fatal_exc("Unknown expression binary type %d", be->type);
    }
    if (left->refer_count == 0) {
        value_free(left);
    }
    if (right->refer_count == 0) {
        value_free(right);
    }
    return result;
}

/**
 *
 * @param ae
 * @param var_table
 * @return
 */
value_t *executor_exp_run_assign(assign_exp_t *ae, hash_t *var_table) {
    value_t *v, *old_v;
    old_v = (value_t *) hash_find(var_table, ae->var, ae->var_len);
    v = executor_exp_run(ae->e, var_table);
    value_incr_quote(v);
    if (is_not_empty(old_v)) {
        if (old_v->refer_count == 1) {
            hash_delete(var_table, ae->var, ae->var_len);
        }
        value_decr_quote(old_v);
    }
    hash_insert_or_update(var_table, ae->var, ae->var_len, (void *) v);
    return v;
}

/**
 *
 * @param pe
 * @param var_table
 * @return
 */
value_t *executor_exp_run_primary(primary_exp_t *pe, hash_t *var_table) {
    char *s;
    int i;
    double d;
    value_t *v;
    switch (pe->type) {
        case exp_value_type_integer:
            i = pe->u.i;
            if (pe->is_sub == 1) {
                i = 0 - i;
            }
            v = value_create_integer(i);
            return v;
        case exp_value_type_string:
            s = (char *) memory_alloc(pe->u.str.len + 1);
            strcpy(s, pe->u.str.val);
            return value_create_string(s);
        case exp_value_type_bool:
            return value_create_bool(pe->u.b);
        case exp_value_type_double:
            d = pe->u.d;
            if (pe->is_sub == 1) {
                d = 0 - d;
            }
            return value_create_double(d);
        case exp_value_type_null:
            return value_create_null();
        case exp_value_type_var:
            v = (value_t *) hash_find(var_table, pe->u.str.val, pe->u.str.len);
            if (is_empty(v)) {
                v = value_create_null();
                hash_insert_or_update(var_table, pe->u.str.val, pe->u.str.len, (void *) v);
            } else {
                if (pe->is_sub == 1) {
                    v = value_duplicate(v, 1);
                }
            }
            return v;
    }
}
#include <asl_core.h>
#include <asl_executor_bin.h>

/**
 * 操作加法
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_plus(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->type = value_type_integer;
        r->u.i = left_i + right_i;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->type = value_type_double;
        r->u.d = left_i + right_d;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->type = value_type_double;
        r->u.d = left_d + right_i;
    } else {
        r->type = value_type_double;
        r->u.d = left_d + right_d;
    }
    return r;
}


/**
 * 操作乘法
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_mul(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->type = value_type_integer;
        r->u.i = left_i * right_i;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->type = value_type_double;
        r->u.d = left_i * right_d;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->type = value_type_double;
        r->u.d = left_d * right_i;
    } else {
        r->type = value_type_double;
        r->u.d = left_d * right_d;
    }
    return r;
}

/**
 * 操作除法
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_div(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->type = value_type_integer;
        r->u.i = left_i / right_i;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->type = value_type_double;
        r->u.d = left_i / right_d;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->type = value_type_double;
        r->u.d = left_d / right_i;
    } else {
        r->type = value_type_double;
        r->u.d = left_d / right_d;
    }
    return r;
}

/**
 * 操作大于
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_gt(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    r->type = value_type_bool;
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->u.b = left_i > right_i ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->u.b = left_i > right_d ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->u.b = left_d > right_i ? TRUE_VAL : FALSE_VAL;
    } else {
        r->u.b = left_d > right_d ? TRUE_VAL : FALSE_VAL;
    }
    return r;
}

/**
 * 操作大于等于
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_ge(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    r->type = value_type_bool;
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->u.b = left_i >= right_i ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->u.b = left_i >= right_d ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->u.b = left_d >= right_i ? TRUE_VAL : FALSE_VAL;
    } else {
        r->u.b = left_d >= right_d ? TRUE_VAL : FALSE_VAL;
    }
    return r;
}

/**
 * 操作小于
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_lt(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    r->type = value_type_bool;
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->u.b = left_i < right_i ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->u.b = left_i < right_d ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->u.b = left_d < right_i ? TRUE_VAL : FALSE_VAL;
    } else {
        r->u.b = left_d < right_d ? TRUE_VAL : FALSE_VAL;
    }
    return r;
}

/**
 * 操作小于等于
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_le(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    r->type = value_type_bool;
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->u.b = left_i <= right_i ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->u.b = left_i <= right_d ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->u.b = left_d <= right_i ? TRUE_VAL : FALSE_VAL;
    } else {
        r->u.b = left_d <= right_d ? TRUE_VAL : FALSE_VAL;
    }
    return r;
}

/**
 * 操作不等于
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_ne(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    r->type = value_type_bool;
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->u.b = left_i != right_i ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->u.b = left_i != right_i ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->u.b = left_i != right_i ? TRUE_VAL : FALSE_VAL;
    } else {
        r->u.b = left_i != right_i ? TRUE_VAL : FALSE_VAL;
    }
    return r;
}

/**
 * 相等操作
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_eq(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    r->type = value_type_bool;
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->u.b = left_i == right_i ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->u.b = left_i == right_d ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->u.b = left_d == right_i ? TRUE_VAL : FALSE_VAL;
    } else if (left_value_type == value_type_string && right_value_type == value_type_string) {
        r->u.b = strcmp(left->u.str.val, right->u.str.val) == 0 ? TRUE_VAL : FALSE_VAL;
    } else {
        r->u.b = left_d == right_i ? TRUE_VAL : FALSE_VAL;
    }
    return r;
}

/**
 * 相等操作
 * @param left
 * @param right
 * @return
 */
value_t *executor_bin_neq(value_t *left, value_t *right) {
    value_t *r = value_create_null();
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    r->type = value_type_bool;
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->u.b = left_i == right_i ? FALSE_VAL : TRUE_VAL;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->u.b = left_i == right_d ? FALSE_VAL : TRUE_VAL;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->u.b = left_d == right_i ? FALSE_VAL : TRUE_VAL;
    } else if (left_value_type == value_type_string && right_value_type == value_type_string) {
        r->u.b = strcmp(left->u.str.val, right->u.str.val) == 0 ? FALSE_VAL : TRUE_VAL;
    } else {
        r->u.b = left_d == right_i ? FALSE_VAL : TRUE_VAL;
    }
    return r;
}

/**
 *
 * @param l
 * @param r
 * @return
 */
value_t *executor_bin_sub(value_t *left, value_t *right) {
    int left_i, right_i;
    double left_d, right_d;
    int left_value_type, right_value_type;
    value_t *r;
    r = value_create_null();
    executor_bin_operate(left, right, &left_i, &right_i, &left_d, &right_d, &left_value_type, &right_value_type);
    if (left_value_type == value_type_integer && right_value_type == value_type_integer) {
        r->type = value_type_integer;
        r->u.i = left_i - right_i;
    } else if (left_value_type == value_type_integer && right_value_type == value_type_double) {
        r->type = value_type_double;
        r->u.d = left_i - right_d;
    } else if (left_value_type == value_type_double && right_value_type == value_type_integer) {
        r->type = value_type_double;
        r->u.d = left_d - right_i;
    } else {
        r->type = value_type_double;
        r->u.d = left_d - right_d;
    }
    return r;
}

/**
 * 操作方法的值
 * @param left
 * @param right
 * @param left_i
 * @param right_i
 * @param left_d
 * @param right_d
 * @param left_value_type
 * @param right_value_type
 */
void executor_bin_operate(value_t *left, value_t *right, int *left_i, int *right_i, double *left_d,
                          double *right_d, int *left_value_type, int *right_value_type) {
    switch (left->type) {
        case value_type_integer:
            *left_i = left->u.i;
            *left_value_type = value_type_integer;
            break;
        case value_type_double:
            *left_d = left->u.d;
            *left_value_type = value_type_double;
            break;
        case value_type_string:
            *left_i = atoi(left->u.str.val);
            *left_value_type = value_type_integer;
            break;
        case value_type_bool:
            *left_i = (int) left->u.b;
            *left_value_type = value_type_integer;
            break;
    }
    switch (right->type) {
        case value_type_integer:
            *right_i = right->u.i;
            *right_value_type = value_type_integer;
            break;
        case value_type_double:
            *right_d = right->u.d;
            *right_value_type = value_type_double;
            break;
        case value_type_string:
            *right_i = atoi(right->u.str.val);
            *right_value_type = value_type_integer;
            break;
        case value_type_bool:
            *right_i = (int) right->u.b;
            *right_value_type = value_type_integer;
            break;
    }
}
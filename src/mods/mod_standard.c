#include <asl_core.h>
#include <asl_module.h>
#include <mod_standard.h>

MOD_ENT_FUNC_ARG_DEF(standard, sleep) = {
        MOD_ENT_FUNC_ARG_BODY("time", LEN("time"), value_type_integer, 1, NULL),
        MOD_ENT_FUNC_EMPTY_ARG_BODY
};
MOD_ENT_FUNC_ARG_DEF(standard, echo) = {
        MOD_ENT_FUNC_ARG_BODY("var", LEN("var"), 0, 1, NULL),
        MOD_ENT_FUNC_EMPTY_ARG_BODY
};
MOD_ENT_FUNCS_DEF(standard) = {
        MOD_ENT_FUNC_BODY("get_time", NULL, MOD_FUNC_NAME(get_time)),
        MOD_ENT_FUNC_BODY("sleep", MOD_ENT_FUNC_ARG_NAME(standard, sleep), MOD_FUNC_NAME(sleep)),
        MOD_ENT_FUNC_BODY("get_version", NULL, MOD_FUNC_NAME(get_version)),
        MOD_ENT_FUNC_BODY("echo", MOD_ENT_FUNC_ARG_NAME(standard, echo), MOD_FUNC_NAME(echo)),
        MOD_ENT_EMPTY_FUNC_BODY,
};

char version[] = ASL_VERSION;
/**
 *
 */
MOD_MOD_BODY(standard);

/**
 * 初始化模块
 * @return
 */
MOD_INIT_FUNC_DEF(standard) {
    return RET_SUCCESS;
}

/**
 * 关闭模块
 * @return
 */
MOD_SHUTDOWN_FUNC_DEF(standard) {
    return RET_SUCCESS;
}

/**
 *
 * @param params
 * @return
 */
MOD_FUNC_DEF(get_time) {
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    return NULL;
}
/**
 *
 * @param params
 * @return
 */
MOD_FUNC_DEF(sleep) {
    value_t *time;
    time = (value_t *) hash_find(var_table, "time", strlen("time"));
    sleep(time->u.i);
    return value_create_null();
}
/**
 *
 * @param params
 * @return
 */
MOD_FUNC_DEF(get_version) {
    char *s;
    s = (char *) memory_alloc(strlen(version) + 1);
    strcpy(s, version);
    return value_create_string(s);
}
/**
 *
 * @param params
 * @param var_table
 * @return
 */
MOD_FUNC_DEF(echo) {
    value_t *v;
    hash_bucket_t *hb;
    hb = var_table->list_head;
    while (hb) {
        v = (value_t *) hb->data;
        switch (v->type) {
            case value_type_integer:
                output_txt("%d", v->u.i);
                break;
            case value_type_null:
                output_txt("null");
                break;
            case value_type_string:
                output_txt("%s", v->u.str.val);
                break;
            case value_type_bool:
                output_txt("%s", v->u.b ? "true" : "false");
                break;
            case value_type_double:
                output_txt("%f", v->u.d);
                break;
            default:
                output_txt("unknown");
                break;

        }
        hb = hb->list_next;
    }
    return value_create_null();
}
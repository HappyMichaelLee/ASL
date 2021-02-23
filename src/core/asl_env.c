#include <asl_core.h>
#include <asl_env.h>

char *env_filter[] = {
        "LS_COLORS",
        NULL,
};
extern char **environ;
env_info_t global_env;

/**
 *
 */
void env_init() {
    char **p = environ;
    char *key, *value;
    global_env.env = hash_create();
    global_env.env_filter = hash_create();
    env_register_filter_env();
    while (*p != NULL) {
        env_parse_equation(*p, &key, &value);
        *p++;
        if (is_not_empty(hash_find(global_env.env_filter, key, strlen(key)))) {
            memory_free(key);
            memory_free(value);
            continue;
        }
        hash_insert_or_update(global_env.env, key, strlen(key), (void *) value);
    }
}

/**
 *
 * @return
 */
int env_register_filter_env() {
    int i;
    i = 0;
    while (env_filter[i] != NULL) {
        hash_insert_or_update(global_env.env_filter, env_filter[i], strlen(env_filter[i]), (void *) env_filter[i]);
        i++;
    }
}

/**
 *
 * @param equation
 * @param key
 * @param value
 * @return
 */
int env_parse_equation(char *equation, char **key, char **value) {
    int len, i, pos;
    char *k, *v;
    len = strlen(equation);
    if (len <= 0) {
        return RET_FAILURE;
    }
    pos = -1;
    for (i = 0; i < len; ++i) {
        if (equation[i] == '=') {
            pos = i;
            break;
        }
    }
    if (pos <= 0 || pos == len - 1) {
        return RET_FAILURE;
    }

    k = (char *) memory_alloc(pos + 1);
    v = (char *) memory_alloc(len - pos);
    util_substr(k, equation, 0, pos);
    util_substr(v, equation, pos + 1, len - pos - 1);
    *key = k;
    *value = v;
    return RET_SUCCESS;
}
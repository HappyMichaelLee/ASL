#ifndef ASL_ASL_ENV_H
#define ASL_ASL_ENV_H


typedef struct env_info_s env_info_t;
struct env_info_s {
    hash_t *env;
    hash_t *env_filter;
};

void env_init();

int env_parse_equation(char *equation, char **key, char **value);

int env_register_filter_env();

#endif
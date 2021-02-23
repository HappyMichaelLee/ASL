#ifndef ASL_ASL_MODULE_H
#define ASL_ASL_MODULE_H


#define MOD_ENT_EMPTY_FUNC_BODY {NULL,NULL,NULL}
#define MOD_ENT_FUNC_BODY(name, args, addr) {name,args,addr}
#define MOD_MOD_BODY(name) module_info_t  mod_##name = {       \
        module_type_func,                                   \
        STR(mod_##name),                                 \
        module_version_v1,                                  \
        mod_entry_funcs_##name,                             \
        mod_init_##name,                                    \
        mod_shutdown_##name                                 \
    }

#define MOD_ENT_FUNC_ARG_DEF(mod_name, func_name) func_param_t mod_entry_func_arg_##mod_name##_##func_name[]
#define MOD_ENT_FUNC_ARG_BODY(name, name_len, force_type, is_required, default_val) {name,name_len,force_type, is_required, default_val}
#define MOD_ENT_FUNC_EMPTY_ARG_BODY {NULL,0,value_type_unknown,-1,NULL}
#define MOD_ENT_FUNCS_DEF(name) module_func_t mod_entry_funcs_##name[]
#define MOD_FUNC_ARG_DEF  hash_t *var_table

#define MOD_FUNC_DEF(name) value_t *mod_func_##name(MOD_FUNC_ARG_DEF)
#define MOD_INIT_FUNC_DEF(name) int mod_init_##name()
#define MOD_SHUTDOWN_FUNC_DEF(name) int mod_shutdown_##name()

#define MOD_ENT_NAME(name) mod_entry_##name
#define MOD_FUNC_NAME(name) mod_func_##name
#define MOD_INIT_FUNC_NAME(name) mod_init_##name
#define MOD_SHUTDOWN_FUNC_NAME(name) mod_shutdown_##name
#define MOD_ENT_FUNC_ARG_NAME(mod_name, func_name) mod_entry_func_arg_##mod_name##_##func_name
#define MOD_MOD_NAME(name) mod_##name

typedef struct module_info_s module_info_t;
typedef struct module_func_s module_func_t;

enum module_type_e {
    module_type_func = 1,
    module_type_class
};

enum module_version_e {
    module_version_v1 = 1,
    module_version_v2
};

struct module_info_s {
    enum module_type_e type;
    const char *name;
    enum module_version_e version;

    const module_func_t *funcs;

    int (*module_init_func)();

    int (*module_shutdown_func)();
};


struct module_func_s {
    char *name;
    func_param_t *args;

    value_t *(*func_addr)(hash_t *var_table);
};

void module_init();

void module_shutdown();

func_t *module_declare_func(char *name, value_t *(*func_addr)(hash_t *), func_param_list_t *params);

#endif
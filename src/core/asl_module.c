#include <asl_core.h>
#include <asl_module.h>

extern module_info_t *global_mods[];

/**
 * 初始化
 */
void module_init() {
    func_t *f;
    const module_func_t *mf;
    module_info_t *m;
    func_param_list_t *fpl;
    int i, ii;
    if (is_empty(global_mods)) {
        return;
    }
    i = ii = 0;
    while (global_mods[i]) {
        m = global_mods[i];
        i++;
        if (is_not_empty(m->module_init_func)) {
            if (m->module_init_func() != RET_SUCCESS) {
                output_warning("module %s initialization failed!", m->name);
            }
        }
        if (is_empty(m->funcs)) {
            continue;
        }
        mf = m->funcs;
        if (is_empty(mf)) {
            continue;
        }
        while (mf->name) {
            if (hash_find(global_compiler.func_table, mf->name, strlen(mf->name))) {
                output_fatal("Internal function:%s has been declared.", mf->name);
            }
            fpl = compiler_create_param_list();
            if (is_not_empty(mf->args)) {
                ii = 0;
                while (is_not_empty(mf->args[ii].name)) {
                    compiler_insert_param_list(fpl, &mf->args[ii]);
                    ii++;
                }
            }
            f = module_declare_func(mf->name, mf->func_addr, fpl);
            hash_insert_or_update(global_compiler.func_table, mf->name, strlen(mf->name), (void *) f);
            mf++;
        }
    }
}

/**
 * 关闭
 */
void module_shutdown() {
    int i;
    module_info_t *m;
    if (is_empty(global_mods)) {
        return;
    }
    m = global_mods[0];
    i = 0;
    while (m) {
        i++;
        if (is_not_empty(m->module_shutdown_func)) {
            if (m->module_shutdown_func() != RET_SUCCESS) {
                output_warning("module %s shutdown failed!", m->name);
            }
        }
        m = global_mods[i];
    }
}

/**
 * 定义
 * @param name
 * @param func_addr
 * @return
 */
func_t *module_declare_func(char *name, value_t *(*func_addr)(hash_t *), func_param_list_t *params) {
    func_t *f;
    f = (func_t *) memory_alloc(sizeof(func_t));
    f->name = name;
    f->type = func_type_native;
    f->func_addr = func_addr;
    f->sl = NULL;
    f->params = params;
    return f;
}
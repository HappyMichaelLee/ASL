#include <asl_core.h>
#include <asl_debugger.h>

extern module_info_t *global_mods[];
extern env_info_t global_env;
extern executor_info_t global_executor;

void debugger_show_compiler() {
    compiler_input_item_t *input_item;
    hash_bucket_t *head;
    int i;
    input_item = global_compiler.input_list->head;
    i = 0;
    while (input_item) {
        output_debug("global_compiler.input_list.%d.filename = %s", i, input_item->filename);
        input_item = input_item->next;
        i++;
    };
    output_debug("global_compiler.enter_filename = %s", global_compiler.enter_filename);
    output_debug("global_compiler.root_path = %s", global_compiler.root_path);

    head = global_compiler.func_table->list_head;
    output_debug("global_compiler.func_table");
    while (head) {
        output_debug("func_name = %s", head->key);
        head = head->list_next;
    }
    head = global_compiler.include_file_table->list_head;
    output_debug("global_compiler.include_file_table");
    while (head) {
        output_debug("include_file = %s", head->key);
        head = head->list_next;
    }
    head = global_compiler.global_table->list_head;
    output_debug("global_compiler.global_table");
    while (head) {
        output_debug("global_name = %s", head->key);
        head = head->list_next;
    }
    head = global_compiler.token_table->list_head;
    output_debug("global_compiler.token_table");
    while (head) {
        output_debug("token = %s", head->data);
        head = head->list_next;
    }
}

void debugger_show_var(hash_t *var_table) {
    hash_bucket_t *head;
    output_debug("global_executor.var_table");
    head = global_executor.var_table->list_head;
    while (head) {
        output_debug("var : %s", head->key);
        debugger_show_value(head->data);
        head = head->list_next;
    }
}

void debugger_show_modules() {
    int i;
    module_info_t *t = global_mods[0];
    i = 0;
    while (t) {
        output_debug("module name : %s", t->name);
        output_debug("module type : %d", t->type);
        output_debug("");
        i++;
        t = global_mods[i];
    }
}

void debugger_show_envs() {
    hash_bucket_t *head;
    head = global_env.env->list_head;
    while (head) {
        output_debug("env: %s = %s", head->key, head->data);
        head = head->list_next;
    }
    output_debug("HOSTNAME = %s", hash_find(global_env.env, "HOSTNAME", strlen("HOSTNAME")));
}

void debugger_show_value(value_t *v) {
    switch (v->type) {
        case value_type_string:
            output_debug("value(string) = %s", v->u.str.val);
            break;
        case value_type_bool:
            output_debug("value(bool) = %s", v->u.b == TRUE_VAL ? "true" : "false");
            break;
        case value_type_double:
            output_debug("value(double) = %f", v->u.d);
            break;
        case value_type_integer:
            output_debug("value(integer) = %d", v->u.i);
            break;
        case value_type_null:
            output_debug("value(null)");
            break;
        default:
            output_debug("unknown(%d)", v->type);
    }
    output_debug("value(refer_count) = %d", v->refer_count);
}
#ifndef ASL_ASL_DEBUGGER_H
#define ASL_ASL_DEBUGGER_H

void debugger_show_compiler();

void debugger_show_modules();

void debugger_show_envs();

void debugger_show_value(value_t *v);

void debugger_show_var(hash_t *var_table);

#endif
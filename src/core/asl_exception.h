#ifndef ASL_ASL_EXCEPTION_H
#define ASL_ASL_EXCEPTION_H

#define EXCEPTION_DEBUG "Debug"
#define EXCEPTION_INFO "Info"
#define EXCEPTION_WARNING "Warning"
#define EXCEPTION_ERROR "Error"
#define EXCEPTION_FATAL "Fatal"

#define exception_fd    stderr

enum exception_level_e {
    exception_level_info = 1,
    exception_level_warning,
    exception_level_error,
    exception_level_fatal
};


void exception_fatal_exc(char *format, ...);

void exception_fatal_compiler_exec(char *format, ...);

#endif

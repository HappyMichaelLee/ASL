#include <asl_core.h>
#include <asl_exception.h>

extern compiler_info_t global_compiler;

/**
 * 错误异常
 * @param format
 * @param ...
 */
void exception_fatal_exc(char *format, ...) {
    va_list args;
    char *time_format = util_get_current_timestamp();
    fprintf(ERROR_FD, "[%s] [%s] ", EXCEPTION_FATAL, time_format);
    va_start(args, format);
    vfprintf(ERROR_FD, format, args);
    va_end(args);
    fprintf(ERROR_FD, EOL);
    memory_free(time_format);
    exit(RET_FAILURE);
}

/**
 * 错误异常
 * @param format
 * @param ...
 */
void exception_fatal_compiler_exec(char *format, ...) {
    va_list args;
    char *time_format = util_get_current_timestamp();
    fprintf(ERROR_FD, "[%s] [%s] ", EXCEPTION_FATAL, time_format);
    va_start(args, format);
    vfprintf(ERROR_FD, format, args);
    va_end(args);
    fprintf(ERROR_FD, EOL);
    fprintf(ERROR_FD, "[%s] [%s] on file '%s' line %d", EXCEPTION_FATAL, time_format, global_compiler.input_item->filename,
            global_compiler.input_item->scan_line + 1);
    fprintf(ERROR_FD, EOL);
    memory_free(time_format);
    exit(RET_FAILURE);
}
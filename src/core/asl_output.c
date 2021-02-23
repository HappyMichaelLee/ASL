#include <asl_core.h>
#include <asl_output.h>

void output_txt(char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(OUTPUT_FD, format, args);
    va_end(args);
    fprintf(OUTPUT_FD, EOL);
}

void output_debug(char *format, ...) {
    va_list args;
    fprintf(OUTPUT_FD, "[%s] ", EXCEPTION_DEBUG);
    va_start(args, format);
    vfprintf(OUTPUT_FD, format, args);
    va_end(args);
    fprintf(OUTPUT_FD, EOL);
}

void output_info(char *format, ...) {
    va_list args;
    fprintf(OUTPUT_FD, "[%s] ", EXCEPTION_INFO);
    va_start(args, format);
    vfprintf(OUTPUT_FD, format, args);
    va_end(args);
    fprintf(OUTPUT_FD, EOL);
}
void output_warning(char *format, ...) {
    va_list args;
    fprintf(OUTPUT_FD, "[%s] ", EXCEPTION_WARNING);
    va_start(args, format);
    vfprintf(OUTPUT_FD, format, args);
    va_end(args);
    fprintf(OUTPUT_FD, EOL);
}
void output_fatal(char *format, ...) {
    va_list args;
    fprintf(ERROR_FD, "[%s] ", EXCEPTION_FATAL);
    va_start(args, format);
    vfprintf(ERROR_FD, format, args);
    va_end(args);
    fprintf(ERROR_FD, EOL);
}
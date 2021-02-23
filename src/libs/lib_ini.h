#ifndef ASL_LIB_INI_H
#define ASL_LIB_INI_H

#define LIB_INI_LINE_SIZE 1024
#define LIB_INI_FILE_SIZE 256

typedef enum ini_line_type_e {
    ini_line_type_idle = 1,
    ini_line_type_error,
    ini_line_type_empty,
    ini_line_type_section,
    ini_line_type_value
};
#endif

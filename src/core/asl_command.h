#ifndef ASL_ASL_COMMAND_H
#define ASL_ASL_COMMAND_H
typedef struct command_info_s command_info_t ;
enum command_type_e {
    command_type_help = 1,
    command_type_ini,
    command_type_version,
    command_type_file
};
struct command_info_s {
    enum command_type_e type;
    int argc;
    char **argv;
    char *bin_file;
    char *root_path;
    union {
        char *filename;
    } u;
};
#endif

#include <asl_core.h>
#include <asl.h>

extern command_info_t global_command;

/**
 * 初始化
 */
void asl_init() {
    setvbuf(stdout, NULL, _IONBF, 0);
}

/**
 *
 * @param argc
 * @param argv
 * @return
 */
void asl_parse_argument(int argc, char **argv) {
    char *filename;
    int opt;
    enum command_type_e type;
    filename = NULL;
    opterr = 0;
    type = command_type_help;
    while ((opt = getopt(argc, argv, "f:hiv")) != -1) {
        switch (opt) {
            case 'f':
                type = command_type_file;
                filename = (char *) memory_alloc(strlen(optarg) + 1);
                strcpy(filename, optarg);
                break;
            case 'i':
                type = command_type_ini;
                break;
            case 'v':
                type = command_type_version;
                break;
            case 'h':
            default:
                type = command_type_help;
                break;
        }
    }
    global_command.bin_file = util_get_realpath(argv[0]);
    global_command.type = type;
    global_command.argc = argc;
    global_command.argv = argv;
    global_command.u.filename = filename;
    global_command.root_path = asl_get_root_path(global_command.bin_file);
}

char *asl_get_root_path(char *bin_file) {
    int i, l, count;
    l = strlen(bin_file);
    count = 0;
    for (i = l - 1; i >= 0; i--) {
        if (bin_file[i] == '/') {
            count++;
            if (count == 2) {
                break;
            }
        }
    }
    return util_substring(bin_file, 0, i);
}

/**
 * 运行帮助
 */
void asl_run_help() {
    output_txt("asls version: %s", ASL_VERSION);
    output_txt("Root path: %s", global_command.root_path);
    output_txt("Usage: %s [-?ivh] [-f filename]", global_command.bin_file);
    output_txt("Options:");
    output_txt("\t-?,-h\t\t\t\t: this help");
    output_txt("\t-v\t\t\t\t: show version and exit");
    output_txt("\t-f filename\t\t\t: run filename");
    output_txt("\t-i\t\t\t\t: show configuration");
    output_txt("");
}

/**
 * 运行版本
 */
void asl_run_version() {
    output_txt("asls version: %s", ASL_VERSION);
    output_txt("");
}

/**
 * 运行配置
 */
void asl_run_ini() {
    output_txt("asls configuration is empty");
    output_txt("");
}

/**
 * 运行文件
 */
void asl_run_file() {
    FILE *input;
    char *start_tm, *end_tm;
    input = fopen(global_command.u.filename, "r");
    if (is_empty(input)) {
        exception_fatal_exc("filename:%s not can be read", global_command.u.filename);
    }
    start_tm = util_get_current_timestamp();
    env_init();
    value_init();
    config_init();
    compiler_init();
    module_init();
    compiler_load_input(global_command.u.filename, input);
    compiler_run();
    executor_init();
    executor_run();
    compiler_unload_input();
    compiler_shutdown();
    executor_shutdown();
    module_shutdown();
    end_tm = util_get_current_timestamp();
    memory_free(start_tm);
    memory_free(end_tm);

}

/**
 * 主
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {
    asl_init();
    asl_parse_argument(argc, argv);
    switch (global_command.type) {
        case command_type_ini:
            asl_run_ini();
            break;
        case command_type_help:
            asl_run_help();
            break;
        case command_type_version:
            asl_run_version();
            break;
        case command_type_file:
            asl_run_file();
            break;
        default:
            asl_run_help();
    }
    return RET_SUCCESS;
}
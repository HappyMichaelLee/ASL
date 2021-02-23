#ifndef ASL_ASL_H
#define ASL_ASL_H

void asl_parse_argument(int argc, char **argv);

void asl_init();

void asl_run_help();

void asl_run_ini();

void asl_run_file();

void asl_run_version();

char *asl_get_root_path(char *bin_file);

#endif

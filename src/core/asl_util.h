#ifndef ASL_ASL_UTIL_H
#define ASL_ASL_UTIL_H

int util_compare_string(char *l, char *r);

char *util_get_current_timestamp();

char *util_substring(char *ch, int pos, int length);

char *util_get_cwd();

char *util_get_realpath(char *filename);

char *util_substr(char *dest, char *src, char start, int n);

#endif

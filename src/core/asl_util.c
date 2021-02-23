#include <asl_core.h>
#include <asl_util.h>

char *util_get_realpath(char *filename) {
    char buf[80], *ret;
    if (realpath(filename, buf)) {
        ret = (char *) memory_alloc(strlen(buf) + 1);
        strcpy(ret, buf);
    } else {
        ret = NULL;
    }
    return ret;
}

/**
 *
 * @return
 */
char *util_get_cwd() {
    char buf[80], *ret;
    getcwd(buf, sizeof(buf));
    ret = (char *) memory_alloc(strlen(buf) + 1);
    strcpy(ret, buf);
    return ret;
}

/**
 *
 * @param l
 * @param r
 * @return
 */
int util_compare_string(char *l, char *r) {
    return strcmp(l, r);
}

/**
 *
 * @return
 */
char *util_get_current_timestamp() {
    int size = 20;
    char *time_format = (char *) memory_alloc(size);
    time_t t;
    struct tm *lt;
    time(&t);
    lt = localtime(&t);
    sprintf(time_format, "%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year + 1900, lt->tm_mon, lt->tm_mday, lt->tm_hour,
            lt->tm_min, lt->tm_sec);
    return time_format;
}

/**
 *
 * @param ch
 * @param pos
 * @param length
 * @return
 */
char *util_substring(char *ch, int pos, int length) {
    char *pch = ch;
    char *subch = (char *) memory_alloc(length + 1);
    int i;
    pch = pch + pos;
    for (i = 0; i < length; i++) {
        subch[i] = *(pch++);
    }
    subch[length] = '\0';
    return subch;
}

char *util_substr(char *dest, char *src, char start, int n) {
    char *p = dest;
    char *q = src;
    char *temp = NULL;
    int len = strlen(src);

    if (start >= len || start < 0) {
        return NULL;
    }
    temp = q + start;
    if (n > strlen(temp)) {
        n = strlen(temp);
    }
    q += start;
    while (n--) *(p++) = *(q++);
    *(p++) = '\0';
    return dest;
}
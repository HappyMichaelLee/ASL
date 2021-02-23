#include <asl_core.h>
#include <asl_memory.h>

/**
 * 内存分配
 */
void *memory_alloc(int size) {
    void *p;
    p = (void *) malloc(size);
    return p;
}

/**
 * 内存释放
 */
void memory_free(void *p) {
    if (is_not_empty(p)) {
        free(p);
    }
}

void memory_copy(void *dest, void *source, size_t size) {
    memcpy(dest, source, size);
}

void memory_set(void *p, int start, int end) {
    memset(p, start, end);
}
#ifndef ASL_ASL_MEMORY_H
#define ASL_ASL_MEMORY_H

void *memory_alloc(int size);

void memory_free(void *p);

void memory_copy(void *dest, void *source, size_t size);

void memory_set(void *p, int start, int end);

#endif

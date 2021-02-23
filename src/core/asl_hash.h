#ifndef ASL_ASL_HASH_H
#define ASL_ASL_HASH_H

#define HASH_MAX_SIZE 0x1000000000
#define HASH_EXPAND_RATIO 0.8
#define HASH_REDUCE_RATIO 0.2
#define HASH_INIT_SIZE 4

typedef struct hash_bucket_s hash_bucket_t;
typedef struct hash_s hash_t;

struct hash_s {
	unsigned int size;
	unsigned int mask;
	unsigned int ele_num;
	float fill_ratio;
	hash_bucket_t *list_head;
	hash_bucket_t *list_tail;
	hash_bucket_t **table;
};

struct hash_bucket_s {
	unsigned long index;
	unsigned int key_len;
	unsigned int index_num;
	char *key;
	void *data;
	hash_bucket_t *list_next;
	hash_bucket_t *list_prev;
	hash_bucket_t *next;
	hash_bucket_t *prev;
};

typedef void (*hash_destroy_callback_func)(void *data);

hash_t *hash_create(void);

hash_bucket_t *hash_create_bucket(char *, unsigned int, int, void *);

void *hash_find(hash_t *, char *, int);

int hash_insert_or_update(hash_t *, char *, unsigned int, void *);

int hash_delete(hash_t *, char *, int);

int hash_expand(hash_t *);

int hash_reduce(hash_t *);

int hash_destroy(hash_t *);

int hash_destroy_callback(hash_t *hash, hash_destroy_callback_func callback);

int hash_index_key(char *key, int);

#endif

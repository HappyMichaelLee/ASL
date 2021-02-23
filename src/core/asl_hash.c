#include <asl_core.h>
#include <asl_hash.h>

/**
 * hash表初始化
 * @return
 */
hash_t *hash_create(void) {
    unsigned int mem_size;
    hash_t *hash;
    hash = (hash_t *) memory_alloc(sizeof(hash_t));
    hash->size = HASH_INIT_SIZE;
    hash->mask = hash->size - 1;
    hash->ele_num = 0;
    hash->fill_ratio = 0.0;
    hash->list_head = NULL;
    hash->list_tail = NULL;
    mem_size = hash->size * sizeof(hash_bucket_t);
    hash->table = (hash_bucket_t **) memory_alloc(mem_size);
    memory_set(hash->table, 0, mem_size);
    return hash;
}

/**
 * 操作哈希桶
 * @param key
 * @param key_len
 * @param index
 * @param data
 * @return
 */
hash_bucket_t *hash_create_bucket(char *key, unsigned int key_len, int index, void *data) {
    hash_bucket_t *new_bucket;
    new_bucket = (hash_bucket_t *) memory_alloc(sizeof(hash_bucket_t));
    new_bucket->next = new_bucket->prev = NULL;
    new_bucket->list_next = new_bucket->list_prev = NULL;
    new_bucket->index_num = 0;
    new_bucket->data = data;
    new_bucket->index = index;
    new_bucket->key = key;
    new_bucket->key_len = key_len;
    return new_bucket;
}

/**
 * hash查找
 * @param hash
 * @param key
 * @return
 */
void *hash_find(hash_t *hash, char *key, int key_len) {
    int index = hash_index_key(key, key_len) % hash->mask;
    hash_bucket_t *bucket;
    bucket = hash->table[index];

    while (bucket) {
        if (util_compare_string(bucket->key, key) == 0) {
            bucket->index_num++;
            return bucket->data;
        }
        bucket = bucket->next;
    }
    return NULL;
}

/**
 * 插入或更新哈希表
 * @param hash
 * @param key
 * @param key_len
 * @param data
 * @return
 */
int hash_insert_or_update(hash_t *hash, char *key, unsigned int key_len, void *data) {
    int ret;
    int hash_index = hash_index_key(key, key_len);
    int index = hash_index % hash->mask;
    hash_bucket_t *bucket, *new_bucket;
    bucket = hash->table[index];
    while (bucket) {
        if (util_compare_string(bucket->key, key) == 0) {
            bucket->data = data;
            return RET_SUCCESS;
        }
        bucket = bucket->next;
    }
    new_bucket = hash_create_bucket(key, key_len, index, data);
    if (hash->table[index]) {
        hash->table[index]->prev = new_bucket;
        new_bucket->next = hash->table[index];
    }
    hash->table[index] = new_bucket;
    hash->ele_num++;
    hash->fill_ratio = (float) hash->ele_num / hash->size;

    if (hash->list_tail) {
        new_bucket->list_prev = hash->list_tail;
        hash->list_tail->list_next = new_bucket;
    }
    hash->list_tail = new_bucket;
    if (hash->ele_num == 1) {
        hash->list_head = new_bucket;
    }
    if (hash->fill_ratio > HASH_EXPAND_RATIO) {
        ret = hash_expand(hash);
        if (ret != RET_SUCCESS) {
            return ret;
        }
    }
    return RET_SUCCESS;
}

/**
 * 哈希删除
 * @param hash
 * @param key
 * @param key_len
 * @return
 */
int hash_delete(hash_t *hash, char *key, int key_len) {
    int opt_ret = RET_FAILURE;
    int index = hash_index_key(key, key_len) % hash->mask;
    hash_bucket_t *bucket;
    bucket = hash->table[index];
    while (bucket) {
        if (util_compare_string(bucket->key, key) == 0) {
            if (bucket->list_prev && bucket->list_next) {
                bucket->list_prev->list_next = bucket->list_next;
                bucket->list_next->list_prev = bucket->list_prev;
            } else if (bucket->list_prev && !bucket->list_next) {
                bucket->list_prev->list_next = NULL;
                hash->list_tail = bucket->list_prev;
            } else if (!bucket->list_prev && bucket->list_next) {
                bucket->list_next->list_prev = NULL;
                hash->list_head = bucket->list_next;
            } else {
                hash->list_head = hash->list_tail = NULL;
            }
            if (bucket->prev && bucket->next) {
                bucket->prev->next = bucket->next;
                bucket->next->prev = bucket->prev;
            } else if (bucket->prev && !bucket->next) {
                bucket->prev->next = NULL;
            } else if (!bucket->prev && bucket->next) {
                bucket->next->prev = NULL;
                hash->table[index] = bucket->next;
            } else {
                hash->table[index] = NULL;
            }
            memory_free(bucket);
            opt_ret = RET_SUCCESS;
            hash->ele_num--;
            hash->fill_ratio = (float) hash->ele_num / hash->size;
            break;
        }
        bucket = bucket->next;
    }
    if (opt_ret == RET_SUCCESS && hash->fill_ratio <= HASH_REDUCE_RATIO) {
        opt_ret = hash_reduce(hash);
        if (opt_ret != RET_SUCCESS) {
            return opt_ret;
        }
    }
    return opt_ret;
}

/**
 * hash扩张
 * @param hash
 * @return
 */
int hash_expand(hash_t *hash) {
    unsigned int mem_size, index;
    hash_bucket_t *bucket, **new_table;
    hash->size = hash->size << 1;
    if (hash->size > HASH_MAX_SIZE) {
        return RET_FAILURE;
    }
    hash->mask = hash->size - 1;

    mem_size = sizeof(hash_bucket_t) * hash->size;
    new_table = (hash_bucket_t **) memory_alloc(mem_size);
    memory_set(new_table, 0, mem_size);
    memory_free(hash->table);
    hash->table = new_table;
    hash->fill_ratio = hash->ele_num / hash->size;
    bucket = hash->list_head;

    while (bucket) {
        bucket->next = bucket->prev = NULL;
        index = hash_index_key(bucket->key, bucket->key_len) % hash->mask;
        bucket->index = index;
        if (hash->table[index]) {
            hash->table[index]->prev = bucket;
            bucket->next = hash->table[index];
        }
        hash->table[index] = bucket;
        bucket = bucket->list_next;
    }
    return RET_SUCCESS;
}

/**
 * 哈希缩编
 * @param hash
 * @return
 */
int hash_reduce(hash_t *hash) {
    unsigned int mem_size, index;
    hash_bucket_t *bucket, **new_table;

    hash->size = hash->size >> 1;
    hash->size = hash->size < HASH_INIT_SIZE ? HASH_INIT_SIZE : hash->size;
    hash->mask = hash->size - 1;

    mem_size = sizeof(hash_bucket_t) * hash->size;
    new_table = (hash_bucket_t **) memory_alloc(mem_size);
    memory_set(new_table, 0, mem_size);
    memory_free(hash->table);
    hash->table = new_table;
    hash->fill_ratio = hash->ele_num / hash->size;
    bucket = hash->list_head;

    while (bucket) {
        bucket->next = bucket->prev = NULL;
        index = hash_index_key(bucket->key, bucket->key_len) % hash->mask;
        bucket->index = index;
        if (hash->table[index]) {
            hash->table[index]->prev = bucket;
            bucket->next = hash->table[index];
        }
        hash->table[index] = bucket;
        bucket = bucket->list_next;
    }
    return RET_SUCCESS;
}

/**
 * hash字符串index
 * @param key
 * @return
 */
int hash_index_key(char *key, int key_len) {
    unsigned int hash = 1315423911;
    unsigned int i = 0;
    for (i = 0; i < key_len; key++, i++) {
        hash ^= ((hash << 5) + (*key) + (hash >> 2));
    }
    return hash;
}

/**
 * 销毁哈希
 * @param hash
 * @return
 */
int hash_destroy(hash_t *hash) {
    hash_bucket_t *bucket, *while_bucket;
    while_bucket = hash->list_head;
    while (while_bucket) {
        bucket = while_bucket;
        memory_free(bucket);
        while_bucket = while_bucket->list_next;
    }
    memory_free(hash);
    return RET_SUCCESS;
}

/**
 * 销毁哈希回调
 * @param hash
 * @param callback
 * @return 
 */
int hash_destroy_callback(hash_t *hash, hash_destroy_callback_func callback) {
    hash_bucket_t *bucket, *while_bucket;
    while_bucket = hash->list_head;
    while (while_bucket) {
        bucket = while_bucket;
        callback(bucket->data);
        memory_free(bucket);
        while_bucket = while_bucket->list_next;
    }
    memory_free(hash);
    return RET_SUCCESS;
}
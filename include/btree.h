#include "stdbool.h"

typedef enum btree_status {
    BTREE_OK    = 0,
    BTREE_ERR   = -1
} btree_status;

typedef struct btree btree;


btree* btree_create(int order);
void   btree_destroy(btree* tree);

btree_status btree_insert(btree* tree, int key);
bool         btree_search(const btree* tree, int key);
btree_status btree_remove(btree* tree, int key);

void         btree_print(const btree* tree);

typedef void (*btree_fn)(int value, void* user);
btree_status btree_foreach(const btree* tree, btree_fn fn, void* user);

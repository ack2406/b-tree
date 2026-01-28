#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

struct btree_node {
    int *keys;
    int  key_count;

    struct btree_node **children;

    bool is_leaf;
};

struct btree {
   struct btree_node* root;
   int t;
};

struct btree_node* btree_node_create(int t, bool is_leaf) {
    struct btree_node* node = malloc(sizeof(*node));
    if (!node) return NULL;

    node->is_leaf = is_leaf;
    node->key_count = 0;

    node->keys = malloc((2 * t - 1) * sizeof(int));
    node->children = malloc(2 * t * sizeof(struct btree_node*));

    for (int i = 0; i < 2 * t; i++) node->children[i] = NULL;

    return node;
}

btree* btree_create(int t) {
    if (t < 2) return NULL;

    btree* tree = malloc(sizeof(*tree));
    if (!tree) return NULL;

    tree->t = t;
    tree->root = btree_node_create(t, true);

    if (!tree->root) { free(tree); return NULL; }

    return tree;
}

void btree_node_destroy(struct btree_node* node) {
    if (!node) return;
    if (!node->is_leaf) {
        for (int i = 0; i <= node->key_count; i++) {
            btree_node_destroy(node->children[i]);
        }
    }

    free(node->keys);
    free(node->children);
    free(node);
}

void btree_destroy(btree* tree) { 
    if (!tree) return;
    btree_node_destroy(tree->root);

    free(tree);
}


void btree_split_child(struct btree_node* parent, int i, int t) {
    struct btree_node* old_child = parent->children[i];
    struct btree_node* new_child = btree_node_create(t, old_child->is_leaf);
    new_child->key_count = t - 1;

    for (int j = 0; j < t - 1; j++) new_child->keys[j] = old_child->keys[j + t];

    if (!new_child->is_leaf) {
        for (int j = 0; j < t; j++) new_child->children[j] = old_child->children[j + t];
    }

    old_child->key_count = t - 1;

    for (int j = parent->key_count; j >= i + 1; j--) parent->children[j + 1] = parent->children[j];
    parent->children[i + 1] = new_child;

    for (int j = parent->key_count - 1; j >= i; j--) parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = old_child->keys[t - 1];
    parent->key_count++;
}

void btree_insert_non_full(struct btree_node* node, int key, int t) {
    int i = node->key_count - 1;

    if (node->is_leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->key_count++;
    } else {
        while (i >= 0 && node->keys[i] > key) i--;
        i++;

        if (node->children[i]->key_count == 2 * t - 1) {
            btree_split_child(node, i, t);

            if (node->keys[i] < key) i++;
        }
    
        btree_insert_non_full(node->children[i], key, t);
    }
}

btree_status btree_insert(btree* tree, int key) {
    if (!tree) return BTREE_ERR;
    
    struct btree_node* root = tree->root;

    if (root->key_count == 2 * tree->t - 1) {
        struct btree_node* new_root = btree_node_create(tree->t, false);
        tree->root = new_root;
        new_root->children[0] = root;
        
        btree_split_child(new_root, 0, tree->t);
        btree_insert_non_full(new_root, key, tree->t);
    } else {
        btree_insert_non_full(root, key, tree->t);
    }

    return BTREE_OK;
}

bool btree_node_search(const struct btree_node* node, int key) {
    if (!node) return false;

    int i = 0;
    while (i < node->key_count && node->keys[i] < key) i++;

    if (i < node->key_count && node->keys[i] == key) return true;
    if (node->is_leaf) return false;
    
    return btree_node_search(node->children[i], key);
}

bool btree_search(const btree* tree, int key) {
    if (!tree || !tree->root) return BTREE_ERR;

    return btree_node_search(tree->root, key);
}

btree_status btree_remove(btree* tree, int key) {
    (void)tree;
    (void)key;
    return BTREE_OK;
}

void btree_node_print(struct btree_node* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; i++) printf("    ");
    
    printf("|--> [");
    for (int i = 0; i < node->key_count; i++) printf(" %d", node->keys[i]);
    printf(" ]\n");

    if (!node->is_leaf) {
        for (int i = 0; i <= node->key_count; i++) {
            btree_node_print(node->children[i], level + 1);
        }
    }
}

void btree_print(const btree* tree) {
    if (!tree || !tree->root) {
        printf("[ ]\n");
        return;
    }

    printf("B-Tree (t=%d):\n", tree->t);
    btree_node_print(tree->root, 0);
}

void btree_node_foreach(struct btree_node* node, btree_fn fn, void* user) {
    if (!node) return;

    int i = 0;
    for (i = 0; i < node->key_count; i++) {
        if (!node->is_leaf) btree_node_foreach(node->children[i], fn, user);
        fn(node->keys[i], user);
    }

    if (!node->is_leaf) btree_node_foreach(node->children[i], fn, user);
}

btree_status btree_foreach(const btree* tree, btree_fn fn, void* user) {
    if (!tree || !tree->root) return BTREE_ERR;
    btree_node_foreach(tree->root, fn, user);
    return BTREE_OK;
}


#include <stdio.h>
#include "btree.h"

void print_item(int key, void* user) {
    (void)user;
    printf("%d ", key);
}

int main(void) {
    btree* tree = btree_create(2);
    if (!tree) return 1;

    int values[] = {1, -1, 21, 37, 69, 420, 67, -123, -67, 5, 7, 21, 25, 50, 100, -51, -26, -21, -100, 999, -999, 112};
    int count = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < count; i++) {
        btree_insert(tree, values[i]);
    }

    btree_print(tree);

    printf("\nSorted values:\n");
    btree_foreach(tree, print_item, NULL);
    printf("\n");

    int search_val = 67;
    if (btree_search(tree, search_val)) {
        printf("\nFound %d\n", search_val);
    } else {
        printf("\n%d not found\n", search_val);
    }

    search_val = 99;
    if (btree_search(tree, search_val)) {
        printf("Found %d\n", search_val);
    } else {
        printf("%d not found\n", search_val);
    }

    btree_destroy(tree);
    return 0;
}

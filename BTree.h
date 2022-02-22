#ifndef B_TREE_BTREE_H
#define B_TREE_BTREE_H

#include "Node.h"

class BTree {
    Node *root;
    int t;
public:
    explicit BTree(int t) : root(nullptr), t(t) {}

    void traverse() { // if b-tree has root, traverse through it
        if (root) {
            root->traverse();
        }
    }

    Node *search(int key) { // search for key if root is not nullptr
        if (!root) {
            return nullptr;
        }
        return root->search(key);
    }

    void insert(int key) { // inserting new key to b-tree
        if (!root) { // if root does not exist, create one
            root = new Node(t, true);
            root->keys[0] = key;
            root->n = 1;
        }
        else {
            if (root->n == 2 * t - 1) { // if root is full, split it
                Node *tmp = new Node(t, false);
                tmp->childs[0] = root;

                tmp->split(0, root);

                int i = 0;
                if (tmp->keys[0] < key) {
                    i++;
                }
                tmp->childs[i]->insert(key);

                root = tmp;
            }
            else { // if not full, just insert to it
                root->insert(key);
            }
        }
    }


    void load(std::string line) { // creating root node and then doing rest of loading
        delete root;
        root = new Node(t, true);
        root->load(line, 2);
    }

    void save() { // just like traverse, but with showing t order at the beginning
        std::cout << t << "\n";
        if (root) {
            root->save();
        }
    }

    int cacheSearch(int key) { // just like traverse, but returns int instead of nodes
        if (!root) {
            return 0;
        }
        return root->cacheSearch(key);
    }

    void cacheSum(std::string line, int cacheSize) { // calculator for cache and no cache traversing
        int *queries = new int[line.size()]; // array for storing queries from line

        int j = 0;

        for (int i = 0; line[i] != '\0'; i++) { // appending queries from line to array
            if (line[i] == ' ') continue;

            int liczba = line[i] - 48;
            for (; line[i + 1] != ' ' && line[i + 1] != '\0'; i++) {
                liczba *= 10;
                liczba += line[i + 1] - 48;
            }

            queries[j] = liczba;
            j++;
        }

        // no cache calculator
        int noCacheSum = 0;

        for (int i = 0; i < j; i++) { // summarizing all steps to get into given key from query
            noCacheSum += cacheSearch(queries[i]);
        }

        std::cout << "NO CACHE: " << noCacheSum << " ";

        // cache calculator
        int cacheSum = 0;


        int *cache = new int[cacheSize]; // array for storing cache

        for (int i = 0; i < j; i++) { // checking if query is in cache already
            bool found = false;
            for (int k = 0; k < cacheSize; k++) {
                if (cache[k] == queries[i]) {
                    found = true;
                    break;
                }
            }

            if (!found) { // if not found, look for it the normal way and add to sum amount of steps
                cacheSum += cacheSearch(queries[i]);
                for (int shift = 0; shift < cacheSize - 1; shift++) {
                    cache[shift] = cache[shift + 1];
                }
                cache[cacheSize - 1] = queries[i];
            }
        }
        std::cout << "CACHE: " << cacheSum << "\n";
        delete[] cache;

    }

    ~BTree() {
        delete root;
    }
};


#endif //B_TREE_BTREE_H

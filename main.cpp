#include <iostream>
#include <string>

#include "BTree.h"

int main() {
    char option;
    std::cin >> option;

    int t;
    std::cin >> t;
    std::cin.ignore(1, '\n');

    BTree tree(t);

    if (option == 'L') { // if chosen option was L, append to b-tree with given line
        std::string line;
        getline(std::cin, line);
        tree.load(line);
    }

    while (scanf("%c", &option) != EOF) {
        switch (option) {
            case 'A' : { // appending new value to b-tree
                int key;
                scanf("%d", &key);
                tree.insert(key);
                break;
            }
            case '?' : { // checking if value is present in b-tree
                int key;
                scanf("%d", &key);
                if (tree.search(key)) {
                    printf("%d +\n", key);
                }
                else {
                    printf("%d -\n", key);
                }
                break;
            }
            case 'P' : { // printing sorted keys
                tree.traverse();
                break;
            }
            case 'S' : { // displaying tree as nodes
                tree.save();
                break;
            }
            case '#' : { // instruction for doing nothing

                break;
            }
            case 'X' : { // instruction for leaving program
                return 0;
            }
            case 'C' : { // cache checking
                int cacheSize;
                std::cin >> cacheSize;
                std::cin.ignore(1, '\n');

                std::string line;
                getline(std::cin, line);

                tree.cacheSum(line, cacheSize);

                break;
            }
            default : {
                break;
            }
        }
    }

    return 0;
}

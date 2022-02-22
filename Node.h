#ifndef B_TREE_NODE_H
#define B_TREE_NODE_H


class Node {
    int *keys;
    int t;
    Node **childs;
    int n;
    bool isLeaf;
public:
    Node(int t, bool isLeaf) : keys(new int[2 * t - 1]), t(t), childs(new Node *[2 * t]), n(0), isLeaf(isLeaf) {}

    void traverse() { // traverse through node to get all keys
        int i;
        for (i = 0; i < n; i++) {
            if (!isLeaf) { // if not leaf, traverse with child on index i
                childs[i]->traverse();
            }
            std::cout << " " << keys[i];
        }
        if (!isLeaf) { // traverse with last child
            childs[i]->traverse();
        }
    }

    Node *search(int key) { // searching for key
        int i;
        for (i = 0; i < n && key > keys[i]; i++); // iterating until key is greater than keys present in node

        if (keys[i] == key) { // return if key was found
            return this;
        }
        if (isLeaf) { // if not found, return nullptr
            return nullptr;
        }
        return childs[i]->search(key); // search for key in
    }

    void insert(int key) { // inserting new key if node is not full
        int i = n - 1;
        if (isLeaf) { // if node is leaf, find place to insert new value
            for (; i >= 0 && keys[i] > key; i--) {
                keys[i + 1] = keys[i];
            }

            keys[i + 1] = key; // insert key at found position
            n++;
        }
        else { // if node is not leaf, find child to which should be key added
            for (; i >= 0 && keys[i] > key; i--);

            if (childs[i + 1]->n == 2 * t - 1) { // if child is full, split it
                split(i + 1, childs[i + 1]);

                if (keys[i + 1] < key) { // check at which split node should new key be
                    i++;
                }
            }
            childs[i + 1]->insert(key);
        }
    }

    void split(int i, Node *splitNode) { // split node if it is full
        Node *tmp = new Node(splitNode->t, splitNode->isLeaf); // new node for splitting
        tmp->n = t - 1; // size half of max

        for (int j = 0; j < t - 1; j++) { // move half of elements to new node
            tmp->keys[j] = splitNode->keys[j + t];
        }
        if (!splitNode->isLeaf) { // move half of nodes to new node as children
            for (int j = 0; j < t; j++) {
                tmp->childs[j] = splitNode->childs[j + t];
            }
        }

        splitNode->n = t - 1; // change size of elements to half for split node

        for (int j = n; j >= i + 1; j--) { // moving nodes to make position for new node
            childs[j + 1] = childs[j];
        }

        childs[i + 1] = tmp; // place node at new position

        for (int j = n - 1; j >= i; j--) { // shift bigger values by one place
            keys[j + 1] = keys[j];
        }

        keys[i] = splitNode->keys[t - 1]; // split node to new position

        n++;
    }

    int load(std::string line, int i) { // loading from line like ( ( 2  4 ) 5 ( 12 ) )
        int childsAmount = 0;

        for (; line[i] != '\0'; i += 2) { // shifting by two places because of spaces
            if (line[i] == '(') { // if ( was found, create new node and go to it
                this->childs[childsAmount] = new Node(t, true);
                this->isLeaf = false;
                childsAmount++;

                i = this->childs[childsAmount - 1]->load(line, i + 2); // returns updated position in line
            }
            else if (line[i] == ')') { // if ) was found, go back with updated i
                return i;
            }
            else if (line[i] >= '0' && line[i] <= '9') { // if number was found, put new key into node
                int liczba = line[i] - 48;
                for (; line[i + 1] != ' '; i++) {
                    liczba *= 10;
                    liczba += line[i + 1] - 48;
                }

                this->keys[n] = liczba;

                this->n++;
            }
        }

        return 1;
    }

    void save() { // like traverse, but with parenthesis
        int i;
        std::cout << "( ";
        for (i = 0; i < n; i++) {
            if (!isLeaf) {
                childs[i]->save();
            }
            std::cout << " " << keys[i] << " ";
        }
        if (!isLeaf) {
            childs[i]->save();
        }
        std::cout << " )";
    }

    int cacheSearch(int key, int sum = 1) { // like search, but with counting memory accesses
        int i;
        for (i = 0; i < n && key > keys[i]; i++);

        if (keys[i] == key || isLeaf) {
            return sum;
        }
        return childs[i]->cacheSearch(key, sum + 1);
    }

    ~Node() {
        delete[] keys;
        delete[] childs;
    };

    friend class BTree;
};


#endif //B_TREE_NODE_H

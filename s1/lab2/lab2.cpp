#include <iostream>
#include <random>
#include <cstring>
#include <fstream>

const size_t MAX_KEY_LEN = 257;

struct TTreap  {
    struct Node {
        Node* left;
        Node* right;

        char* x;
        uint64_t value;              

        int y;                       

        
        Node(const char* k, uint64_t v) {
            left = nullptr;
            right = nullptr;

            x = new char[MAX_KEY_LEN];
            memcpy(x, k, MAX_KEY_LEN);

            value = v;

            y = rand();
        }

        
        ~Node() {
            delete[] x;
        }
    };

    using TNode = Node*;

    TNode root;

    TTreap () {
        root = nullptr;
    }
    
    void Free(TNode tree) {
        if (tree == nullptr) {
            return;
        }
        Free(tree->left);
        Free(tree->right);
        delete tree;
    }
    ~TTreap () {
        Free(root);
    }
    
    TNode Merge(TNode leftT, TNode rightT) {
        if (leftT == nullptr) {
            return rightT;
        }
        if (rightT == nullptr) {
            return leftT;
        }

        if (leftT->y > rightT->y) {
            leftT->right = Merge(leftT->right, rightT);
            return leftT;
        }
        else {
            rightT->left = Merge(leftT, rightT->left);
            return rightT;
        }
    }
    
    void Split(TNode tree, const char* x, TNode& leftT, TNode& rightT) {
        if (tree == nullptr) {
            leftT = nullptr;
            rightT = nullptr;
            return;
        }

        if (strcmp(tree->x, x) < 0) {
            Split(tree->right, x, tree->right, rightT);
            leftT = tree;
        }
        else {
            Split(tree->left, x, leftT, tree->left);
            rightT = tree;
        }
    }

    TNode Min(TNode ptr) {
        while (ptr != nullptr && ptr->left != nullptr) {
            ptr = ptr->left;
        }
        return ptr;
    }
    
    TNode Search(TNode tree, const char* x) {
        if (tree == nullptr) {
            return nullptr;
        }

        int isKey = strcmp(tree->x, x);

        if (isKey < 0) {
            return Search(tree->right, x);
        }
        else if (isKey > 0) {
            return Search(tree->left, x);
        }
        else {
            return tree;
        }
    }

    void Find(const char* x) {
        TNode elem = Search(root, x);
        if (elem != nullptr) {
            std::cout  << "OK: " << elem->value << "\n";
            return;
        }
        std::cout  << "NoSuchWord\n";
    }

    void Insert(const char* x, uint64_t value) {
        TNode leftT = nullptr;
        TNode rightT = nullptr;
        TNode elem = nullptr;

        Split(root, x, leftT, rightT);

        elem = Min(rightT);

        if (elem != nullptr && strcmp(elem->x, x) == 0) {
            std::cout  << "Exist\n";
            root = Merge(leftT, rightT);
            return;
        }

        TNode newNode = new Node(x, value);

        root = Merge(Merge(leftT, newNode), rightT);
        std::cout  << "OK\n";
    }

    void Remove(char* x) {
        TNode leftT = nullptr;
        TNode rightT = nullptr;
        TNode subLeftT = nullptr;
        TNode subRightT = nullptr;

        Split(root, x, leftT, rightT);

        size_t len = strlen(x);
        x[len] = 1;
        x[len + 1] = '\0';

        Split(rightT, x, subLeftT, subRightT);

        if (subLeftT != nullptr) {
            std::cout  << "OK\n";
            root = Merge(leftT, subRightT);
            delete subLeftT;
            return;
        }

        std::cout  << "NoSuchWord\n";
        rightT = Merge(subLeftT, subRightT);
        root = Merge(leftT, rightT);

    }
};

void ToLower(char* string) {
    for (size_t i = 0; string[i] != '\0'; ++i) {
        if ('A' <= string[i] && string[i] <= 'Z') {
            string[i] = string[i] - 'A' + 'a';
        }
    }
}

int main() {
    TTreap tree;
    char input[MAX_KEY_LEN] = { 0 };
    //std::ifstream in("D:/Study/discr_an/lab2/test.txt");

    uint64_t value;
    while (std::cin >> input) {
        switch (input[0]) {
            case '+':
                std::cin >> input >> value;
                ToLower(input);
                tree.Insert(input, value);
                break;

            case '-':
                std::cin >> input;
                ToLower(input);
                tree.Remove(input);
                break;

            default:
                ToLower(input);
                tree.Find(input);
                break;
        }
    }
    return 0;
}
/*
Реализовать декартово дерево с возможностью поиска, добавления и удаления элементов.

Необходимо создать программную библиотеку, реализующую указанную структуру данных,
на основе которой разработать программу-словарь.
В словаре каждому ключу, представляющему из себя регистронезависимую
последовательность букв английского алфавита длиной не более 256 символов,
поставлен в соответствие некоторый номер, от 0 до 264 - 1.
Разным словам может быть поставлен в соответствие один и тот же номер.

Программа должна обрабатывать строки входного файла до его окончания.
 Каждая строка может иметь следующий формат:

+ word 34 — добавить слово «word» с номером 34 в словарь.
 Программа должна вывести строку «OK», если операция прошла успешно,
 «Exist», если слово уже находится в словаре.

- word — удалить слово «word» из словаря.
 Программа должна вывести «OK», если слово существовало и было удалено,
 «NoSuchWord», если слово в словаре не было найдено.

word — найти в словаре слово «word».
 Программа должна вывести «OK: 34», если слово было найдено; число, которое следует за «OK:» — номер, присвоенный слову при добавлении.
 В случае, если слово в словаре не было обнаружено, нужно вывести строку «NoSuchWord».

Команд ! Save и ! Load в тестах нет.
*/

#include <iostream>
#include <cstdint>
#include <cstring>
#include <random>

const short MAX_KEY_LEN = 260;

class TTreap {
    struct Node {
        Node* left;
        Node* right;

        char* x;            // key
        uint64_t value;     // value

        int y;              // priority

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

public:
    TTreap() {
        root = nullptr;
    };

    void Free(TNode tree) {
        if (tree == nullptr) {
            return;
        }
        Free(tree->left);
        Free(tree->right);
        delete tree;
    }
    ~TTreap() {
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

    void Split(TNode tree, const char* key, TNode& leftT, TNode& rightT) {
        if (tree == nullptr) {
            leftT = nullptr;
            rightT = nullptr;
            return;
        }
        if (strcmp(tree->x, key) < 0) {
            Split(tree->right, key, tree->right, rightT);
            leftT = tree;
        }
        else {
            Split(tree->left, key, leftT, tree->left);
            rightT = tree;
        }
    }

    TNode Min(TNode ptr) {
        while (ptr != nullptr && ptr->left != nullptr) {
            ptr = ptr->left;
        }
        return ptr;
    }

    TNode Search(TNode tree, const char* key) {
        if (tree == nullptr) {
            return nullptr;
        }

        int isKey = strcmp(tree->x, key);
        if (isKey < 0) {
            return Search(tree->right, key);
        }
        else if (isKey > 0) {
            return Search(tree->left, key);
        }
        else {
            return tree;
        }
     }

    void Find(const char* key) {
        TNode element = Search(root, key);
        if (element != nullptr) {
            std::cout << "OK: " << element->value << std::endl;
            return;
        }
        std::cout << "NoSuchWord\n";
    }

    void Insert(const char* key, uint64_t value) {
        TNode leftT = nullptr;
        TNode rightT = nullptr;
        TNode element = nullptr;

        Split(root, key, leftT, rightT);

        element = Min(rightT);
        if (element != nullptr && strcmp(element->x, key) == 0) {
            std::cout << "Exist\n";
            root = Merge(leftT, rightT);
            return;
        }

        TNode newNode = new Node(key, value);
        root = Merge(Merge(leftT, newNode), rightT);
        std::cout << "OK\n";
    }

    void Remove(char* key) {
        TNode leftT = nullptr;
        TNode rightT = nullptr;

        Split(root, key, leftT, rightT);

        TNode subLeftT = nullptr;
        TNode subRightT = nullptr;

        size_t len = strlen(key);
        key[len] = 1;                   // немного увеличиваем ключ, чтобы
        key[len + 1] = '\0';            // при повторном разделении оригинальный узел ушёл влево

        Split(rightT, key, subLeftT, subRightT);

        if (subLeftT != nullptr) {
            std::cout << "OK" << std::endl;

            root = Merge(leftT, subRightT);
            delete subLeftT;
            return;
        }

        std::cout << "NoSuchWord" << std::endl;

        subRightT = Merge(leftT, rightT);
        root = Merge(subLeftT, subRightT);
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
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    TTreap tree;
    char input[MAX_KEY_LEN];
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
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

struct Node {
    int begin;
    int end;
    Node* suffixLink;
    std::map<char, Node*> connections;

    Node() {
        begin = 0;
        end = 0;
        suffixLink = nullptr;
    }
    Node(int b, int e) {
        begin = b;
        end = e;
        suffixLink = nullptr;
    }

    ~Node() {
        for (auto& child : connections) {
            delete child.second;
        }
    }
};

class Trie {
private:
    std::string str;
    Node* root;
    Node* lastAdd;
    Node* current;
    
    int remain = 0;
    int edge = 0;
    int edgeSize = 0;

public:
    Trie() {
        root = nullptr;
        lastAdd = nullptr;
        current = nullptr;

        remain = 0;
        edge = 0;
        edgeSize = 0;
    }

    ~Trie() {
        delete root;
    }

    void Build(std::string &newstr) {
        str = newstr;
        int strSize = str.size();

        root = new Node(strSize, strSize);
        lastAdd = root;
        current = root;
        root->suffixLink = root;

        for(int i = 0; i < strSize; i++) {
            lastAdd = root;
            remain++;

            while(remain) {
                Node* newNode;
                if(edgeSize == 0) {
                    edge = i;
                }
                std::map<char, Node*>::iterator pos = current->connections.find(str[edge]);
                if (pos == current->connections.end()) {
                    newNode = new Node(i, strSize);
                    current->connections[str[edge]] = newNode;
                    if (lastAdd != root) {
                        lastAdd->suffixLink = current;
                    }
                    lastAdd = current;
                }
                else {
                    newNode = pos->second;
                    int newEdgeSize = std::min(i + 1, newNode->end) - newNode->begin;
                    if (edgeSize >= newEdgeSize) {
                        edge += newEdgeSize;
                        edgeSize -= newEdgeSize;
                        current = newNode;
                        continue;
                    }
                    else if (str[newNode->begin + edgeSize] == str[i]) {
                        edgeSize++;
                        if(lastAdd != root) {
                            lastAdd->suffixLink = current;
                        }
                        lastAdd = current;
                        break;
                    }
                    else {
                        Node* split = new Node(newNode->begin, newNode->begin + edgeSize);
                        Node* tmp = new Node(i, strSize);
                        current->connections[str[edge]] = split;
                        split->connections[str[i]] = tmp;

                        newNode->begin += edgeSize;
                        split->connections[str[newNode->begin]] = newNode;
                        if(lastAdd != root) {
                            lastAdd->suffixLink = split;
                        }
                        lastAdd = split;
                    }
                }
                remain--;
                if(current == root && edgeSize) {
                    edgeSize--;
                    edge = i - remain + 1;
                }
                else {
                    current = current->suffixLink;
                }
            }
        }
    }

    std::vector<int> Search(std::string &substr) {
        std::vector<int> res;
        int iter = 0;
        Node* currSearch = root;

        int strSize = str.size();
        int substrSize = substr.size();

        bool drop = false;
        int i = 0;
        while(i < substrSize){
            std::map<char, Node*>::iterator pos = currSearch->connections.find(substr[i]);
            if(pos == currSearch->connections.end()){
                drop = true;
                break;
            }
            else {
                currSearch = pos->second;
                iter += currSearch->end - currSearch->begin;
                int j = 0;
                for(j = 0; i + j < substrSize && currSearch->begin + j < currSearch->end; j++) {
                    if(str[currSearch->begin + j] != substr[i + j]){
                        drop = true;
                        break;
                    }
                }
                i += j;
                if(i == substrSize){
                    break;
                }
            }
        }

        if(substrSize < strSize && !drop) {
            SearchRecurrent(currSearch, res, iter, strSize);
        }
        sort(res.begin(), res.end());
        return res;
    }

    void SearchRecurrent(Node* curr, std::vector<int> &res, int pos, int strSize) {
        if(curr->end == strSize) {
            res.push_back(strSize - pos + 1);
        }
        else {
            for(const auto &node : curr->connections) {
                SearchRecurrent(node.second, res, pos + node.second->end - node.second->begin, strSize);
            }
        }
    }
};

void Print(std::vector<int> res, int count) {
    int resSize = res.size();
    if(resSize == 0){
        return;
    }
    else {
        std::cout << count << ": ";
        for(int i = 0; i < resSize; i++) {
            std::cout << res[i];
            if(i == resSize - 1) {
                std::cout << "\n";
                return;
            }
            else {
                std::cout << ", ";
            }
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    Trie trie;
    std::string str;
    std::cin >> str;
    str += "$";
    trie.Build(str);

    int count = 1;
    std::string substr;
    while(std::cin >> substr) {
        Print(trie.Search(substr), count);
        count++;
    }
    return 0;
}
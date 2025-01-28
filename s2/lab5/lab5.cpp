#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

struct TNode {
    int begin;
    int end;
    TNode* suffixLink;
    std::map<char, TNode*> connections;

    TNode() {
        begin = 0;
        end = 0;
        suffixLink = nullptr;
    }
    TNode(int b, int e) {
        begin = b;
        end = e;
        suffixLink = nullptr;
    }

    ~TNode() {
        for (auto& child : connections) {
            delete child.second;
        }
    }
};

class TTrie {
private:
    std::string str;
    TNode* root;
    TNode* lastAdd;
    TNode* current;

    int remain = 0;
    int edge = 0;
    int edgeSize = 0;

public:
    TTrie() {
        root = nullptr;
        lastAdd = nullptr;
        current = nullptr;

        remain = 0;
        edge = 0;
        edgeSize = 0;
    }

    ~TTrie() {
        delete root;
    }

    void Build(std::string &newstr) {
        str = newstr;
        int strSize = str.size();

        root = new TNode(strSize, strSize);
        lastAdd = root;
        current = root;
        root->suffixLink = root;

        for(int i = 0; i < strSize; i++) {
            lastAdd = root;
            remain++;

            while(remain) {
                TNode* newTNode;
                if(edgeSize == 0) {
                    edge = i;
                }
                std::map<char, TNode*>::iterator pos = current->connections.find(str[edge]);
                if (pos == current->connections.end()) {
                    newTNode = new TNode(i, strSize);
                    current->connections[str[edge]] = newTNode;
                    if (lastAdd != root) {
                        lastAdd->suffixLink = current;
                    }
                    lastAdd = current;
                }
                else {
                    newTNode = pos->second;
                    int newEdgeSize = std::min(i + 1, newTNode->end) - newTNode->begin;
                    if (edgeSize >= newEdgeSize) {
                        edge += newEdgeSize;
                        edgeSize -= newEdgeSize;
                        current = newTNode;
                        continue;
                    }
                    else if (str[newTNode->begin + edgeSize] == str[i]) {
                        edgeSize++;
                        if(lastAdd != root) {
                            lastAdd->suffixLink = current;
                        }
                        lastAdd = current;
                        break;
                    }
                    else {
                        TNode* split = new TNode(newTNode->begin, newTNode->begin + edgeSize);
                        TNode* tmp = new TNode(i, strSize);
                        current->connections[str[edge]] = split;
                        split->connections[str[i]] = tmp;

                        newTNode->begin += edgeSize;
                        split->connections[str[newTNode->begin]] = newTNode;
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
        TNode* currSearch = root;

        int strSize = str.size();
        int substrSize = substr.size();

        bool drop = false;
        int i = 0;
        while(i < substrSize){
            std::map<char, TNode*>::iterator pos = currSearch->connections.find(substr[i]);
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

    void SearchRecurrent(TNode* curr, std::vector<int> &res, int pos, int strSize) {
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

    TTrie trie;
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
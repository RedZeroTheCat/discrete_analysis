#include <iostream>
#include <array>
#include <algorithm>
#include <climits>
#include <memory>

#pragma GCC optimize("O3")

const int MAX_D = 10;
const int MAX_N = 100000;

struct point {
    std::array<int, MAX_D> coord{};
    int index;

    point() = default;

    point(const std::array<int, MAX_D>& coords, int idx) : coord(coords), index(idx) {}
};

struct Comparator {
    int axis;
    explicit Comparator(int ax) : axis(ax) {}

    bool operator()(const point& a, const point& b) const {
        return a.coord[axis] < b.coord[axis];
    }
};

class KDTree {
public:
    struct KDNode {
        point dot;
        std::unique_ptr<KDNode> left = nullptr;
        std::unique_ptr<KDNode> right = nullptr;

        KDNode(const std::array<int, MAX_D>& coords, int idx) : dot(coords, idx) {}
    };
private:
    using NodePtr = std::unique_ptr<KDNode>;

    NodePtr root = nullptr;
    int dim;

    void recursive_insert(NodePtr& node, const std::array<int, MAX_D>& coords, int index, int depth) {
        const int axis = depth % dim;
        if (coords[axis] <= node->dot.coord[axis]) {
            if (node->left == nullptr) {
                node->left = std::make_unique<KDNode>(coords, index);
            } else {
                recursive_insert(node->left, coords, index, depth + 1);
            }
        } else {
            if (node->right == nullptr) {
                node->right = std::make_unique<KDNode>(coords, index);
            } else {
                recursive_insert(node->right, coords, index, depth + 1);
            }
        }
    }

    bool euc_d2(const std::array<int, MAX_D>& x, const std::array<int, MAX_D>& y, long long& min_d) {
        long long result = 0;
        for (int i = 0; i < dim; i++) {
            long long tmp = x[i] - y[i];
            result += tmp * tmp;
            if (result >= min_d) {
                return false;
            }
        }
        min_d = result;
        return true;
    }

    void find_nearest(NodePtr& node, const std::array<int, MAX_D>& query, int depth, long long& min_d, int& best_ind) {
        const int axis = depth % dim;
        if (euc_d2(node->dot.coord, query, min_d)) {
            best_ind = node->dot.index;
        }

        bool is_left = true;
        if (query[axis] <= node->dot.coord[axis]) {
            if (node->left != nullptr) {
                find_nearest(node->left, query, depth + 1, min_d, best_ind);
            }
        }
        else {
            if (node->right != nullptr) {
                find_nearest(node->right, query, depth + 1, min_d, best_ind);
            }
            is_left = false;
        }

        // Проверка, может ли в другом поддереве быть более близкая точка
        if ((is_left && node->right != nullptr) || (!is_left && node->left != nullptr)) {
            long long tmp = query[axis] - node->dot.coord[axis];
            if (tmp * tmp < min_d) {
                if (is_left && node->right != nullptr) {
                    find_nearest(node->right, query, depth + 1, min_d, best_ind);
                }
                else if (node->left != nullptr) {
                    find_nearest(node->left, query, depth + 1, min_d, best_ind);
                }
            }
        }
    }
public:
    explicit KDTree(int d) : dim(d) {}

    void insert(const std::array<int, MAX_D>& coords, int index) {
        if (root == nullptr) {
            root = std::make_unique<KDNode>(coords, index);
        } else {
            recursive_insert(root, coords, index, 0);
        }
    }

    int nearest(const std::array<int, MAX_D>& query) {
        if (root == nullptr) {
            return -1;
        }

        long long min_d = LLONG_MAX;
        int index = -1;
        find_nearest(root, query, 0, min_d, index);
        return index;
    }
};

void build(KDTree& tree, std::array<point, MAX_N>& coords, int l, int r, int depth, int dim) {
    if (r <= l) {
        return;
    }
    if (r - l == 1) {
        tree.insert(coords[l].coord, coords[l].index);
        return;
    }
    int axis = depth % dim;
    int mid = (l + r) / 2;
    std::nth_element(coords.begin() + l, coords.begin() + mid, coords.begin() + r, Comparator(axis));
    tree.insert(coords[mid].coord, coords[mid].index);
    build(tree, coords, l, mid, depth + 1, dim);
    build(tree, coords, mid + 1, r, depth + 1, dim);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    int n, d;
    std::cin >> n >> d;
    KDTree tree(d);
    std::array<point, MAX_N> coords;

    for (int i = 0; i < n; i++) {
        std::array<int, MAX_D> x{};
        for (int j = 0; j < d; j++) {
            std::cin >> x[j];
        }
        coords[i] = point(x, i + 1);
    }

    build(tree, coords, 0, n, 0, d);

    int q;
    std::cin >> q;
    for (int i = 0; i < q; i++) {
        std::array<int, MAX_D> query{};
        for (int j = 0; j < d; j++) {
            std::cin >> query[j];
        }
        std::cout << tree.nearest(query) << '\n';
    }
}

// ПОИСК БЛИЖАЙШИХ СОСЕДЕЙ
/* Условие:
 Дано множество точек в многомерном пространстве.
 В каждом запросе задается точка, Вам необходимо вывести номер
 ближайшей к ней точки из исходного множества в смысле простого евклидова расстояния.

 Если ближайших точек несколько, то выведите номер любой из них.
 Читать сразу все запросы и обрабатывать их одновременно запрещено.

 * Формат ввода:
 В первой строке задано два числа 1 <= n <= 10^5 и 1 <= d <= 10 - количество точек в множестве и размерность пространства.
 Каждая из следующих n строк содержит d целых чисел x[i][j] - j-я координата i-й точки, |x[i][j]| <= 10^8.

 Далее следует целое число 1 <= q <= 10^6 - количество запросов
 Каждая из следующих q строк содержит d целых чисел y[i][j] - j-я координата i-й точки, |y[i][j]| <= 10^8.

 * Формат вывода:
 Для каждого запроса выведите в отдельной строке единственное число — индекс ближайшей точки из множества.
 Если таких несколько, выведите любую.
 Индексация точек множества начинается с 1. <- !!!!!!!!!!

 Max:
 x[i][j] - y[i][j] = 2 * 10^8
 (x[i][j] - y[i][j])^2 = 4 * 10^16
 sqrt((x[i][j] - y[i][j])^2) = 2 * 10^8
 */

/*
// Быстрое возведение в степень для целых чисел и положительной степени
int bin_pow(int num, int pow) {
    int result;
    if (pow == 1) {
            return num;
    }
    if (pow == 0) {
        return 1;
    }

    if (pow % 2 == 0) {
        result = bin_pow(num, pow / 2);
        return result * result;
    }
    else {
        result = bin_pow(num, (pow - 1) / 2);
        return num * result * result;
    }
}
*/

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

// k-мерное дерево
struct KDNode {
    std::vector<int> coord;
    int index;
    KDNode* left;
    KDNode* right;

    KDNode(const std::vector<int>& point, int idx) {
        coord = point;
        index = idx;
        left = nullptr;
        right = nullptr;
    }
};

class KDTree {
private:
    KDNode* root;

    KDNode* build(std::vector<std::pair<std::vector<int>, int>>& coords, int depth, int left, int right, int dim) {
        if (left >= right) {
            return nullptr;
        }
        int axis = depth % dim;
        int mid = (left + right) / 2;

        auto comparator = [axis](const std::pair<std::vector<int>, int>& a, const std::pair<std::vector<int>, int>& b) {
            return a.first[axis] < b.first[axis];
        };

        // Сортировка подмассива по текущей оси
        std::nth_element(coords.begin() + left,
                         coords.begin() + mid,
                         coords.begin() + right,
                         comparator);

        KDNode* node = new KDNode(coords[mid].first, coords[mid].second);
        node->left = build(coords, depth + 1, left, mid, dim);
        node->right = build(coords, depth + 1, mid + 1, right, dim);

        return node;
    }

    // Квадрат Евклидова расстояния
    long long euc_d2(const std::vector<int>& x, const std::vector<int>& y, int dim) {
        long long sum = 0;
        for (int i = 0; i < dim; ++i) {
            long long tmp = x[i] - y[i];
            sum += tmp * tmp;
        }
        return sum;
    }

    void find_nearest(KDNode* node, const std::vector<int>& query, int depth, long long& min_d, int& index, int dim) {
        if (!node) {
            return;
        }
        long long d = euc_d2(query, node->coord, dim);
        if (d < min_d) {
            min_d = d;
            index = node->index;
        }

        int axis = depth % dim;
        KDNode* next;
        KDNode* other;

        if (query[axis] < node->coord[axis]) {
            next = node->left;
            other = node->right;
        }
        else {
            next = node->right;
            other = node->left;
        }
        find_nearest(next, query, depth + 1, min_d, index, dim);

        // Проверка, может ли в другом поддереве быть более близкая точка
        long long tmp = query[axis] - node->coord[axis];
        if (tmp * tmp < min_d) {
            find_nearest(other, query, depth + 1, min_d, index, dim);
        }
    }

public:
    KDTree(const std::vector<std::vector<int>>& coords, int size, int dim) {
        std::vector<std::pair<std::vector<int>, int>> points(size);
        for (int i = 0; i < size; ++i) {
            points[i] = {coords[i], i + 1};
        }
        root = build(points, 0, 0, size, dim);
    }

    int nearest(const std::vector<int>& query, int dim) {
        long long min_d = LLONG_MAX;
        int index = -1;
        find_nearest(root, query, 0, min_d, index, dim);
        return index;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    int n, d;
    std::cin >> n >> d;

    std::vector<std::vector<int>> coords(n, std::vector<int>(d));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < d; ++j) {
            std::cin >> coords[i][j];
        }
    }
    KDTree tree(coords, n, d);

    int q;
    std::cin >> q;

    std::vector<int> y(d);
    for (int i = 0; i < q; ++i) {
        for (int j = 0; j < d; ++j) {
            std::cin >> y[j];
        }
        std::cout << tree.nearest(y, d) << "\n";
    }
}

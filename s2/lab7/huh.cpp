#include <iostream>
#include <vector>

const int maxN = 100;
const int maxM = 5000;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    int n, m;
    std::cin >> n >> m;

    std::vector<int> w(n);
    std::vector<long long> c(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> w[i] >> c[i];
    }

    long long ans = 0;
    std::vector<std::vector<long long>> res(n + 1, std::vector<long long>(m + 1));
    return 0;
}
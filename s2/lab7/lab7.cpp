#include <iostream>
#include <vector>

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

    std::vector<std::vector<long long>> ansPrev(n + 1, std::vector<long long>(m + 1));
    std::vector<std::vector<std::vector<bool>>> resPrev(n + 1, std::vector<std::vector<bool>>(m + 1, std::vector<bool>(n, false)));

    long long ans = 0;
    std::vector<bool> res(n, false);

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            ansPrev[i][j] = ansPrev[i - 1][j];
            resPrev[i][j] = resPrev[i - 1][j];

            if (j >= w[i - 1] && c[i - 1] > ansPrev[i][j]) {
                ansPrev[i][j] = c[i - 1];
                resPrev[i][j] = std::vector<bool>(n, false);
                resPrev[i][j][i - 1] = true;
            }

            if (ansPrev[i][j] > ans) {
                ans = ansPrev[i][j];
                res = resPrev[i][j];
            }
        }
    }
    std::vector<std::vector<long long>> ansCur(n + 1, std::vector<long long>(m + 1));
    std::vector<std::vector<std::vector<bool>>> resCur(n + 1, std::vector<std::vector<bool>>(m + 1, std::vector<bool>(n, false)));

    for (long long i = 2; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            for (int k = 1; k <= m; ++k) {
                ansCur[j][k] = ansCur[j - 1][k];
                resCur[j][k] = resCur[j - 1][k];

                if (k >= w[j - 1] && ansPrev[j - 1][k - w[j - 1]] > 0) {
                    long long newCost = i * (c[j - 1] + ansPrev[j - 1][k - w[j - 1]] / (i - 1));
                    if (newCost > ansCur[j][k]) {
                        ansCur[j][k] = newCost;
                        resCur[j][k] = resPrev[j - 1][k - w[j - 1]];
                        resCur[j][k][j - 1] = true;
                    }
                }
                if (ansCur[j][k] > ans) {
                    ans = ansCur[j][k];
                    res = resCur[j][k];
                }
            }
        }
        std::swap(ansCur, ansPrev);
        std::swap(resCur, resPrev);
    }

    std::cout << ans << '\n';
    for (int i = 0; i < n; ++i) {
        if (res[i]) {
            std::cout << i + 1 << ' ';
        }
    }
    std::cout << '\n';

    return 0;
}

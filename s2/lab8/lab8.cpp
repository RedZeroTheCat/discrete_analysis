#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    long n;
    long m;
    std::vector<std::tuple<long, long, long>> segments; // {L[i], R[i], i}

    std::cin >> n;
    for (long i = 0; i < n; ++i) {
        long l, r;
        std::cin >> l >> r;
        segments.emplace_back(l, r, i);
    }
    std::cin >> m;

    std::sort(segments.begin(), segments.end());

    long currentEnd = 0;
    long maxEnd = 0;
    std::vector<std::tuple<long, long, long>> chosen;

    int i = 0;
    while (currentEnd < m) {
        bool found = false;
        long bestSegInd = -1;

        while (i < n && std::get<0>(segments[i]) <= currentEnd) {
            if (std::get<1>(segments[i]) > maxEnd) {
                maxEnd = std::get<1>(segments[i]);
                bestSegInd = i;
                found = true;
            }
            ++i;
        }

        if (!found) {
            std::cout << "0" << std::endl;
            return 0;
        }

        chosen.emplace_back(std::get<0>(segments[bestSegInd]),
                            std::get<1>(segments[bestSegInd]),
                            std::get<2>(segments[bestSegInd]));
        currentEnd = maxEnd;
    }

    std::sort(chosen.begin(), chosen.end(),
              [](const auto& a, const auto& b) { return std::get<2>(a) < std::get<2>(b); });

    std::cout << chosen.size() << "\n";
    for (const auto& [l, r, ind] : chosen) {
        std::cout << l << ' ' << r << '\n';
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <climits>

const long long INF = LLONG_MAX;

struct TEdge {
    long long begin;
    long long end;
    long long weight;

    TEdge(long long b, long long e, long long w) {
        begin = b;
        end = e;
        weight = w;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    long long n, m, start, finish;
    std::cin >> n >> m >> start >> finish;

    std::vector<TEdge> graph;
    long long edgeBegin, edgeEnd, edgeWeight;
    for (long long i = 0; i < m; i++) {
        std::cin >> edgeBegin >> edgeEnd >> edgeWeight;
        graph.emplace_back(edgeBegin, edgeEnd, edgeWeight);
    }

    std::vector<long long> solve(n, INF);
    solve[start - 1] = 0;
    bool change;
    while (true) {
        change = false;
        for (long long i = 0; i < m; i++) {
            if (solve[graph[i].begin - 1] < INF) {
                if (solve[graph[i].end - 1] > solve[graph[i].begin - 1] + graph[i].weight) {
                    solve[graph[i].end - 1] = solve[graph[i].begin - 1] + graph[i].weight;
                    change = true;
                }
            }
        }

        if (!change) {
            break;
        }
    }
    if (solve[finish - 1] == INF) {
        std::cout << "No solution" << std::endl;
    }
    else {
        std::cout << solve[finish - 1] << std::endl;
    }
    return 0;
}
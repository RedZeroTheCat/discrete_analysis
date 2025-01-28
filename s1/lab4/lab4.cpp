#include <iostream>
#include <vector>
#include <string>

std::vector<int> ZFunc(const std::string & s) {
    int n = s.size();
    std::vector<int> z(n);
    z[0] = n;
    int l = -1, r = -1;
    for (int i = 1; i < n; i++) {
        if (i <= r) {
            z[i] = std::min(z[i - l], r - i + 1);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string pattern, text;
    std::cin >> text;
    std::cin >> pattern;

    text = pattern + '#' + text;
    std::vector<int> zVector = ZFunc(text);

    unsigned long textSize = text.size();
    unsigned long patternSize = pattern.size();

    for (unsigned long i = 0; i < textSize; i++) {
        if (zVector[i] == patternSize) {
            std::cout << i - patternSize - 1 << std::endl;
        }
    }
    return 0;
}
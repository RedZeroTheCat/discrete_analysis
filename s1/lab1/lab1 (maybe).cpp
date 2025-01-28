/*
 Требуется разработать программу, осуществляющую ввод пар «ключ-значение»,
 их упорядочивание по возрастанию ключа указанным алгоритмом сортировки
 за линейное время и вывод отсортированной последовательности.
 Вариант задания определяется типом ключа (и соответствующим ему методом сортировки) и типом значения:

 Сортировка подсчётом.
 Тип ключа: числа от 0 до 65535.
 Тип значения: строки фиксированной длины 64 символа, во входных данных могут встретиться строки меньшей длины,
               при этом строка дополняется до 64-х нулевыми символами, которые не выводятся на экран.
 */

#include <iostream>
#include <cstring>

const unsigned short MAX_KEY = 65535;
const unsigned short VALUE_SIZE = 65;
const size_t BASE_SIZE = 10000;

class TPair {
public:
    TPair() : key(0), value() {}

    TPair(unsigned short k, char v[VALUE_SIZE]) : key(k), value(v) {}

    TPair operator=(TPair p) {
        key = p.key;
        value = p.value;
        return *this;
    }

    unsigned short  GetKey() {
        return key;
    }

    void Print() {
        std::cout << key << '\t' << value << '\n';
    }

private:
    unsigned short key;
    char* value;
};

void Resize(TPair*& arrayUnit, size_t& size) {
    size_t newSize = size * 2;
    TPair* newArrayUnit = new TPair[newSize];

    for (size_t i = 0; i < size; i++) {
        newArrayUnit[i] = arrayUnit[i];
    }
    delete[] arrayUnit;
    arrayUnit = newArrayUnit;
    size = newSize;
}

void CountingSort(TPair* arrayUnit, TPair*& newArrayUnit,
                  size_t size, unsigned short max, size_t*& counts)
{
    for (size_t i = 0; i <= max; i++) {
        counts[i] = 0;
    }
    for (size_t i = 0; i < size; i++) {
        counts[arrayUnit[i].GetKey()]++;
    }
    for (size_t i = 1; i <= max; i++) {
        counts[i] += counts[i - 1];
    }
    for (size_t i = size; i > 0; i--) {
        newArrayUnit[counts[arrayUnit[i - 1].GetKey()] - 1] = arrayUnit[i - 1];
        counts[arrayUnit[i - 1].GetKey()]--;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t size = 10000;
    TPair* arrayUnit = new TPair[size];

    unsigned short key = 0;
    char value[VALUE_SIZE] = "";

    unsigned short max = 0;
    size_t iteration = 0;

    bool anyExist = false;
    while (std::cin >> key) {
        std::cin.ignore();
        std::cin.getline(value, VALUE_SIZE);
        anyExist = true;
        if (key > max) {
            max = key;
        }
        if (iteration >= BASE_SIZE) {
            Resize(arrayUnit, size);
        }
        arrayUnit[iteration] = {key, value};
        iteration++;
    }

    if (!anyExist) {
        delete[] arrayUnit;
        return 0;
    }

    TPair* newArrayUnit = new TPair[iteration];
    size_t* counts = new size_t[max + 1];
    CountingSort(arrayUnit, newArrayUnit, iteration, max,  counts);

    for (size_t i = 0; i < iteration; i++) {
        newArrayUnit[i].Print();
    }

    delete[] arrayUnit;
    /*for (size_t i = 0; i < iteration; i++) {
        delete[] arrayValue[i];
    }
    delete[] arrayValue; */
    delete[] newArrayUnit;
    //delete[] newArrayValue;
    delete[] counts;
    return 0;
}
/*
 *
5 ghjfghfjg
3 jgfjg
1 kgjfgk
2 kdjfklgj
3 dkgjkdg
 * */
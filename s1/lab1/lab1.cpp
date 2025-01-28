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

#include <chrono>
#include <fstream>

const unsigned short MAX_KEY = 65535;
const unsigned short VALUE_SIZE = 65;
const size_t BASE_SIZE = 10000;


void Resize(unsigned short*& arrayKey, char**& arrayValue, size_t& size) {
    size_t newSize = size * 2;
    unsigned short* newArrayKey = new unsigned short[newSize];
    char** newArrayValue = new char *[newSize];

    for (size_t i = 0; i < size; i++) {
        newArrayKey[i] = arrayKey[i];
        newArrayValue[i] = new char[VALUE_SIZE];
        strcpy(newArrayValue[i], arrayValue[i]);
    }
    delete[] arrayKey;
    for (size_t i = 0; i < size; i++) {
        delete[] arrayValue[i];
    }
    delete[] arrayValue;
    arrayKey = newArrayKey;
    arrayValue = newArrayValue;
    size = newSize;
}

void CountingSort(unsigned short*& arrayKey, char**& arrayValue,
                  unsigned short*& newArrayKey, char**& newArrayValue,
                  size_t size, unsigned short max, size_t*& counts)
{
    for (size_t i = 0; i <= max; i++) {
        counts[i] = 0;
    }
    for (size_t i = 0; i < size; i++) {
        counts[arrayKey[i]]++;
    }
    for (size_t i = 1; i <= max; i++) {
        counts[i] += counts[i - 1];
    }
    for (size_t i = size; i > 0; i--) {
        newArrayKey[counts[arrayKey[i - 1]] - 1] = arrayKey[i - 1];
        newArrayValue[counts[arrayKey[i - 1]] - 1] = arrayValue[i - 1];
        counts[arrayKey[i - 1]]--;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::ifstream in("D:/Study/discr_an/lab1/test.txt");

    size_t size = BASE_SIZE;
    unsigned short* arrayKey = new unsigned short[size];
    char** arrayValue = new char*[size];

    unsigned short key = 0;
    char value[VALUE_SIZE] = "";

    unsigned short max = 0;
    size_t iteration = 0;

    bool anyExist = false;
    while (in >> key) {
        in.ignore();
        in.getline(value, VALUE_SIZE);
        anyExist = true;
        if (key > max) {
            max = key;
        }
        if (iteration >= size) {
            Resize(arrayKey, arrayValue, size);
        }
        arrayKey[iteration] = key;
        arrayValue[iteration] = new char[VALUE_SIZE];
        strcpy(arrayValue[iteration], value);
        iteration++;
    }

    if (!anyExist) {
        delete[] arrayKey;
        delete[] arrayValue;
        return 0;
    }

    unsigned short* newArrayKey = new unsigned short[iteration];
    char** newArrayValue = new char *[iteration];
    size_t* counts = new size_t[max + 1];

    auto start = std::chrono::high_resolution_clock::now();
    CountingSort(arrayKey, arrayValue, newArrayKey, newArrayValue,iteration, max,  counts);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "Counting sort duration: " << duration.count() << std::endl << std::endl;

    /*for (size_t i = 0; i < iteration; i++) {
        std::cout << newArrayKey[i] << "\t" << newArrayValue[i] << "\n";
    }*/

    delete[] arrayKey;
    for (size_t i = 0; i < iteration; i++) {
        delete[] arrayValue[i];
    }
    delete[] arrayValue;
    delete[] newArrayKey;
    delete[] newArrayValue;
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
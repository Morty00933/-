#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <functional>

void sumArrayPart(const std::vector<int>& array, int start, int end, int partNumber, int& result) {
    result = std::accumulate(array.begin() + start, array.begin() + end, 0);
    std::cout << "Part " << partNumber << ": sum = " << result << std::endl;
}

int main() {
    int n = 100;
    std::vector<int> array(n);
    for (int i = 0; i < n; ++i) {
        array[i] = i + 1; // Заполняем массив числами от 1 до n
    }

    int numParts = 4;
    int partSize = n / numParts;
    std::vector<int> results(numParts, 0);
    std::vector<std::thread> threads;

    for (int i = 0; i < numParts; ++i) {
        int start = i * partSize;
        int end = (i == numParts - 1) ? n : start + partSize;
        threads.emplace_back(sumArrayPart, std::cref(array), start, end, i + 1, std::ref(results[i]));
    }

    int totalSum = 0;

    for (int i = 0; i < numParts; ++i) {
        threads[i].join();
        totalSum += results[i];
    }

    std::cout << "Total sum: " << totalSum << std::endl;

    return 0;
}

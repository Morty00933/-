#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex mtx;
const int numThreads = 4;
const int dataSize = 100;
std::vector<int> data(dataSize, 1);  // Заполняем вектор значениями 1

void processData(int threadId, int startIdx, int endIdx) {
    int localSum = 0;
    for (int i = startIdx; i < endIdx; ++i) {
        localSum += data[i];
    }

    // Синхронизированный доступ к общему ресурсу
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Thread " << threadId << " processed data from index " << startIdx << " to " << endIdx 
              << " with local sum: " << localSum << std::endl;
}

int main() {
    std::vector<std::thread> threads;
    int partSize = dataSize / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startIdx = i * partSize;
        int endIdx = (i == numThreads - 1) ? dataSize : startIdx + partSize;
        threads.emplace_back(processData, i + 1, startIdx, endIdx);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads have finished processing." << std::endl;
    return 0;
}

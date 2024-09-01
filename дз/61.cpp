#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<int> globalCounter(0);

void increment(int numIterations) {
    for (int i = 0; i < numIterations; ++i) {
        globalCounter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    const int numThreads = 5;
    const int numIterations = 10000;

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(increment, numIterations);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final value of globalCounter: " << globalCounter.load(std::memory_order_relaxed) << std::endl;

    return 0;
}

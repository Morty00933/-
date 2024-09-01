#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <fstream>
#include <stdexcept>
#include <string>

std::mutex mtx;

void task(int threadNum, int duration) {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Имитация выполнения задачи
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Thread " << threadNum << " with id: " << std::this_thread::get_id()
              << " finished in " << elapsed.count() << " seconds.\n";
}

int main(int argc, char* argv[]) {
    int numThreads = 5;
    if (argc > 1) {
        try {
            numThreads = std::stoi(argv[1]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid argument: " << e.what() << "\n";
            return 1;
        }
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        try {
            threads.emplace_back(task, i + 1, (i + 1) * 100);
        } catch (const std::exception& e) {
            std::cerr << "Error creating thread " << i + 1 << ": " << e.what() << "\n";
        }
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}

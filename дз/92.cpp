#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

class Barrier {
public:
    Barrier(int numThreads) : totalThreads(numThreads), count(0) {}

    // Метод для инициализации барьера
    void initBarrier() {
        count = 0;
    }

    // Метод для синхронизации работы потоков
    void synchronize() {
        std::unique_lock<std::mutex> lock(mutex);

        count++;
        if (count < totalThreads) {
            cv.wait(lock);
        } else {
            cv.notify_all();
        }
    }

private:
    int totalThreads;
    int count;
    std::mutex mutex;
    std::condition_variable cv;
};

class DataProcessor {
public:
    DataProcessor(std::vector<int>& data, Barrier& barrier) : data(data), barrier(barrier) {}

    // Метод для обработки данных
    void processData(int threadID) {
        // Обработка данных
        std::cout << "Thread " << threadID << " processing data..." << std::endl;

        // Синхронизация работы потоков
        barrier.synchronize();

        // Дополнительная обработка данных после синхронизации
        std::cout << "Thread " << threadID << " continues processing data..." << std::endl;
    }

private:
    std::vector<int>& data;
    Barrier& barrier;
};

int main() {
    const int numThreads = 3;
    std::vector<std::thread> threads;
    std::vector<int> data(100); // Пример большого массива данных
    Barrier barrier(numThreads);

    // Инициализация барьера
    barrier.initBarrier();

    // Создание потоков и запуск обработки данных
    for (int i = 0; i < numThreads; ++i) {
        DataProcessor processor(data, barrier);
        threads.emplace_back(&DataProcessor::processData, &processor, i);
    }

    // Ожидание завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "All threads finished processing data." << std::endl;

    return 0;
}

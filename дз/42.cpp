#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <numeric>
#include <random>
#include <chrono>

// Размер массива и количество случайных чисел для генерации
const int arraySize = 100;
const int numRandomNumbers = 100;

std::vector<int> array(arraySize);
std::atomic<bool> dataReady(false);
std::atomic<int> currentIndex(0);

void generateRandomNumbers() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    for (int i = 0; i < numRandomNumbers; ++i) {
        int randomNumber = dist(mt);
        int index = currentIndex.fetch_add(1, std::memory_order_relaxed);

        if (index < arraySize) {
            array[index] = randomNumber;
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Имитируем задержку
        } else {
            break;
        }
    }
    
    dataReady.store(true, std::memory_order_release);
}

void calculateSum() {
    while (!dataReady.load(std::memory_order_acquire)) {
        // Ожидаем, пока dataReady не станет true
        std::this_thread::yield();
    }

    int sum = std::accumulate(array.begin(), array.end(), 0);
    std::cout << "Sum of array elements: " << sum << std::endl;
}

int main() {
    std::thread producer(generateRandomNumbers);
    std::thread consumer(calculateSum);

    producer.join();
    consumer.join();

    return 0;
}

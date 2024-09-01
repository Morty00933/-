#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ParallelQueue {
public:
    // Добавление элемента в очередь
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(value);
        cv.notify_one(); // Уведомляем потоки, ожидающие извлечения элемента
    }

    // Извлечение элемента из очереди
    T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return !queue.empty(); }); // Ожидаем, пока очередь не станет непустой
        T value = queue.front();
        queue.pop();
        return value;
    }

    // Проверка, пуста ли очередь
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }

private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable cv;
};

int main() {
    ParallelQueue<int> queue;

    // Поток, добавляющий элементы в очередь
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            queue.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Имитация задержки
        }
    });

    // Поток, извлекающий элементы из очереди
    std::thread consumer([&]() {
        while (true) {
            if (!queue.empty()) {
                int value = queue.pop();
                std::cout << "Popped value: " << value << std::endl;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Имитация задержки
            }
        }
    });

    producer.join();
    consumer.join();

    return 0;
}

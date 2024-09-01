#include <iostream>
#include <stack>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeStack {
public:
    // Добавление элемента в стек
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        stack.push(value);
        cv.notify_one(); // Уведомляем потоки, ждущие извлечения элемента
    }

    // Извлечение элемента из стека
    T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this] { return !stack.empty(); }); // Ожидаем, пока стек не станет непустым
        T value = stack.top();
        stack.pop();
        return value;
    }

    // Проверка, пуст ли стек
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return stack.empty();
    }

private:
    std::stack<T> stack;
    mutable std::mutex mutex;
    std::condition_variable cv;
};

int main() {
    ThreadSafeStack<int> stack;

    // Поток, добавляющий элементы в стек
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            stack.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Имитация задержки
        }
    });

    // Поток, извлекающий элементы из стека
    std::thread consumer([&]() {
        while (true) {
            if (!stack.empty()) {
                int value = stack.pop();
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

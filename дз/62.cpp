#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <string>

template <typename T>
class AsyncEventQueue {
public:
    // Добавляет событие в очередь
    void push(const T& event) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(event);
        cv.notify_one();
    }

    // Ожидает и возвращает следующее событие из очереди
    std::future<T> wait() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty(); });
        
        T event = queue.front();
        queue.pop();
        
        return std::async(std::launch::deferred, [event] { return event; });
    }

    // Возвращает текущий размер очереди
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }

    // Проверяет, пуста ли очередь событий
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }

private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::queue<T> queue;
};

int main() {
    AsyncEventQueue<std::string> eventQueue;

    // Добавление событий в очередь
    eventQueue.push("Event 1");
    eventQueue.push("Event 2");
    eventQueue.push("Event 3");

    // Ожидание и получение событий из очереди
    auto futureEvent1 = eventQueue.wait();
    auto futureEvent2 = eventQueue.wait();
    auto futureEvent3 = eventQueue.wait();

    // Вывод полученных событий на экран
    std::cout << "Received: " << futureEvent1.get() << std::endl;
    std::cout << "Received: " << futureEvent2.get() << std::endl;
    std::cout << "Received: " << futureEvent3.get() << std::endl;

    // Проверка размера очереди
    std::cout << "Queue size: " << eventQueue.size() << std::endl;

    // Проверка, пуста ли очередь
    if (eventQueue.empty()) {
        std::cout << "Queue is empty" << std::endl;
    } else {
        std::cout << "Queue is not empty" << std::endl;
    }

    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
int currentThread = 0;

void printMessage(const std::string& message, int threadNumber) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] { return threadNumber == currentThread; });

    std::cout << message << std::endl;

    currentThread++;
    cv.notify_all();
}

int main() {
    std::thread t1(printMessage, "Hello from thread 1!", 0);
    std::thread t2(printMessage, "Hello from thread 2!", 1);
    std::thread t3(printMessage, "Hello from thread 3!", 2);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Hello, Parallel World!" << std::endl;

    return 0;
}

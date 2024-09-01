#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm> // Для std::sort
#include <utility>   // Для std::move

class VectorContainer {
public:
    std::vector<int> vec;

    VectorContainer(std::vector<int> v) : vec(std::move(v)) {}
    
    void sort() {
        std::sort(vec.begin(), vec.end());
    }
};

std::mutex mtx;
VectorContainer container({10, 2, 33, 4, 15, 6, 17, 8});

void sortVectorInThread() {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Sorting vector in secondary thread...\n";
    container.sort();
}

int main() {
    std::cout << "Original vector:\n";
    for (int n : container.vec) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // Запуск второго потока для сортировки вектора
    std::thread sorter(sortVectorInThread);
    sorter.join();

    // Вывод отсортированного вектора в главном потоке
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Sorted vector:\n";
        for (int n : container.vec) {
            std::cout << n << " ";
        }
        std::cout << "\n";
    }

    return 0;
}

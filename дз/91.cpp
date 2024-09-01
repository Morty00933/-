#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class TextEditor {
public:
    TextEditor(const std::string& text) : text(text) {}

    // Метод для инициализации мьютекса
    void initMutex() {
        mutex.lock();
    }

    // Метод для запуска потоков
    void startThreads() {
        // Запускаем два потока для одновременного редактирования текста
        threads.emplace_back(&TextEditor::editText, this, 0, 5, "Thread 1");
        threads.emplace_back(&TextEditor::editText, this, 5, 10, "Thread 2");
    }

    // Метод для синхронизации работы потоков
    void synchronizeThreads() {
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        mutex.unlock();
    }

    // Метод, который редактирует текст в указанном диапазоне
    void editText(int start, int end, const std::string& threadName) {
        // Блокируем доступ к тексту в указанном диапазоне
        mutex.lock();

        std::cout << "Thread " << threadName << " editing text: ";
        for (int i = start; i < end; ++i) {
            text[i] = toupper(text[i]);
            std::cout << text[i];
        }
        std::cout << std::endl;

        // Разблокируем доступ к тексту в указанном диапазоне
        mutex.unlock();
    }

    // Метод для вывода текста
    void displayText() const {
        std::cout << "Current text: " << text << std::endl;
    }

private:
    std::string text;
    std::vector<std::thread> threads;
    std::mutex mutex;
};

int main() {
    // Создаем экземпляр класса TextEditor с исходным текстом
    TextEditor editor("hello world");

    // Инициализируем мьютекс для доступа к тексту
    editor.initMutex();

    // Запускаем два потока для редактирования текста
    editor.startThreads();

    // Синхронизируем работу потоков
    editor.synchronizeThreads();

    // Выводим отредактированный текст
    editor.displayText();

    return 0;
}

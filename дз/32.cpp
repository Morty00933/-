#include <iostream>
#include <thread>
#include <stdexcept>
#include <string>

class Printer {
public:
    Printer(int count, const std::string& text) : count(count), text(text) {
        if (count < 0) {
            throw std::invalid_argument("Count cannot be negative");
        }
        if (text.empty()) {
            throw std::invalid_argument("Text cannot be empty");
        }
    }

    void print() const {
        for (int i = 0; i < count; ++i) {
            std::cout << text << std::endl;
        }
    }

private:
    int count;
    std::string text;
};

void threadFunction(Printer printer) {
    printer.print();
}

void processPrinters(const Printer& printer1, const Printer& printer2) {
    std::thread t1(threadFunction, printer1);
    std::thread t2(threadFunction, printer2);

    t1.join();
    t2.join();
}

int main() {
    try {
        Printer printer1(3, "Hello, World!");
        Printer printer2(2, "Goodbye, World!");

        processPrinters(printer1, printer2);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

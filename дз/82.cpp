#include <iostream>
#include <atomic>
#include <memory>
#include <vector>
#include <thread>

template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& data) : data(data), next(nullptr) {}
    };

    std::atomic<Node*> topNode;

public:
    Stack() : topNode(nullptr) {}

    ~Stack() {
        while (auto* oldTop = topNode.load()) {
            topNode.store(oldTop->next);
            delete oldTop;
        }
    }

    void push(const T& data) {
        Node* newNode = new Node(data);
        newNode->next = topNode.load();
        while (!topNode.compare_exchange_weak(newNode->next, newNode));
    }

    std::shared_ptr<T> pop() {
        Node* oldTop = topNode.load();
        while (oldTop && !topNode.compare_exchange_weak(oldTop, oldTop->next));
        if (!oldTop) {
            return nullptr;
        }
        std::shared_ptr<T> res = std::make_shared<T>(oldTop->data);
        delete oldTop;
        return res;
    }

    std::shared_ptr<T> top() const {
        Node* oldTop = topNode.load();
        if (oldTop) {
            return std::make_shared<T>(oldTop->data);
        } else {
            return nullptr;
        }
    }
};

void testStack(Stack<int>& stack, int numIterations) {
    for (int i = 0; i < numIterations; ++i) {
        stack.push(i);
    }
}

int main() {
    const int numThreads = 4;
    const int numIterations = 10000;

    Stack<int> stack;

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(testStack, std::ref(stack), numIterations);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Stack size: " << numThreads * numIterations << std::endl;

    return 0;
}

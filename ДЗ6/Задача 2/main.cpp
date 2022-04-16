#include <queue>
#include <mutex>
#include <exception>
#include <thread>
#include <iostream>


template<typename T>
class ThreadsafeQueue {
    std::queue<T> queue_;
    mutable std::mutex mutex_;
public:
    ThreadsafeQueue() = default;
    ThreadsafeQueue(const ThreadsafeQueue<T>&) = delete;
    ThreadsafeQueue& operator=(const ThreadsafeQueue<T>&) = delete;

    ThreadsafeQueue(ThreadsafeQueue<T>&& other) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_ = std::move(other.queue_);
    }

    virtual ~ThreadsafeQueue() { }

    bool empty() const {
        return queue_.empty();
    }

    unsigned long size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    //убираем последний элемент
    void pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (empty()) {
            return;
        }
        queue_.pop();
    }

    //добавляем элемент в очередь
    void push(const T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(item);
    }

    //возвращаем первый элемент очереди
    T front()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            throw std::logic_error("No elements in queue");
        }
        return queue_.front();
    }

    //возвращаем последний элемент очереди
    T back()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            throw std::logic_error("No elements in queue");
        }
        return queue_.back();
    }
};

int main()
{
    ThreadsafeQueue<int> queue;
    auto push_back = [&queue]()
    {
        for (int i = 0; i < 10; ++i)
        {
            queue.push(i);
        }
    };
    std::thread t1(push_back); // поток для записи в очередь
    auto pop = [&queue]()
    {
        for (int i = 0; i < 10; ++i)
        {
            queue.pop();
        }
    };
    std::thread t2(pop); // поток для удаления элементов из очереди
    t1.join();
    t2.join();
    if (queue.empty())
        std::cout << "Everything is OK!";
    else std::cout << "Some problems";
    return 0;
}

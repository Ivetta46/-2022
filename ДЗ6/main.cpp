#include <iostream>
#include <numeric>
#include <vector>
#include <thread>
#include <mutex>


std::mutex mtx;

template <typename Iterator, typename T>
void accumulate_wrapper(Iterator begin, Iterator end, T & result) {
    T sum = std::accumulate(begin, end, 0);
    mtx.lock();
    result += sum;
    mtx.unlock();
}

template <typename Iterator, typename T>
void parallel_accumulate(Iterator begin, Iterator end, T & result) {
    result = 0;
    // Размер диапазона
    auto size = std::distance(begin, end);
    // Количество исполнителей
    auto num_workers = std::thread::hardware_concurrency();
    // Проверяем, нужно ли распараллеливать
    if (size < num_workers * 4) {
        result = std::accumulate(begin, end, 0);
        return;
    }
    // Распараллеливать всё-таки нужно, тогда:
    // Считаем количество данных на одного исполнителя
    auto size_per_worker = size / num_workers;
    // Создаём два вектора: с потоками и для записи результатов
    std::vector<std::thread> threads;
    // Распределяем данные и запускаем потоки.
    for(auto i = 0u; i < num_workers - 1; i++) {
        threads.push_back(std::thread(accumulate_wrapper<Iterator, T>,
                                      std::next(begin, i * size_per_worker), // сдвиг begin
                                      std::next(begin, (i + 1) * size_per_worker), // сдвиг begin
                                      std::ref(result))); // для записи результата
    }
    // Производим расчёт и в основном потоке остатка последовательности
    auto sum = std::accumulate(
            std::next(begin, (num_workers - 1) * size_per_worker),
            end, 0);

    mtx.lock();
    result += sum;
    mtx.unlock();

    // Ждём, пока остальные потоки завершат работу
    for(auto & thread : threads) {
        thread.join();
    }
}

int main() {
    std::vector<int> sequence(100);
    //std::vector<int> sequence(100000);
    std::iota(std::begin(sequence), std::end(sequence), 0);
    long int result;
    parallel_accumulate( std::begin(sequence), std::end(sequence), result );
    std::cout << result << std::endl;
}

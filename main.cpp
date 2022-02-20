#include <iostream>
#include <vector>
#include <stack>
#include <array>
#include <chrono>
#include <windows.h>

using namespace std::chrono;

template<typename Type>
class Timer
{
private:
    using clock_t = std::chrono::steady_clock;

    std::chrono::time_point<clock_t> m_beg;
    bool pause;
    std::stack<long long> interval;

public:
    Timer()
    {
        m_beg = clock_t::now();
        pause = false;
    }

// перезапуск таймера
    void reset()
    {
        m_beg = clock_t::now();
        pause = false;
        interval = std::stack<long long>();
    }

// Приостановить/продолжить замер(Если замер был приостановлен, то продолжим, иначе - приостановим)
    void pause_continue(bool b)
    {
        if (!pause)
            interval.push(std::chrono::duration_cast<Type>(clock_t::now() - m_beg).count());
        pause = !pause;
        m_beg = clock_t::now();
    }

// вывести текущее значение замера, при этом замер приостанавливается
    long long vuvod()
    {
        long long tmp = 0;

// если замер ранее не был приостановлен, то подсчитываем временной интервал и останавливаем
        if (!pause) {
            tmp = std::chrono::duration_cast<Type>(clock_t::now() - m_beg).count();
            pause_continue(true); // приостанавливаем замер
        }

// подсчет суммарного интервала приостановленных ранее замеров
        while (!interval.empty()) {
            tmp += interval.top();
            interval.pop();
        }
        interval.push(tmp);
        return tmp;
    }
};

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    const int k = 10000; // количество всех элементов тестируемого массива
    std::array<int, k> arX;

    std::cout << "Начало замера" << std::endl;
    Timer<std::chrono::nanoseconds> t1;
    Timer<std::chrono::microseconds> t2;
    Timer<std::chrono::milliseconds> t3;

    for (int i = 0; i < k; ++i) {
        arX[i] = i + 1;
        std::cout << arX[i] << std::endl;
    }

    std::cout << "Приостановка замера" << std::endl;
    t1.pause_continue(false);
    t2.pause_continue(false);
    t3.pause_continue(false);
    for (int i = 0; i < k; ++i)
        arX[i] -= 1;

    std::cout << "Продолжение замера" << std::endl;
    t1.pause_continue(false);
    t2.pause_continue(false);
    t3.pause_continue(false);
    for (int i = 0; i < k; ++i)
        arX[i] += 2;

    std::cout << "Приостановка замера" << std::endl;
    t1.pause_continue(false);
    t2.pause_continue(false);
    t3.pause_continue(false);
    for (int i = 0; i < k; ++i)
        arX[i] += 3;

    std::cout << "Продолжение замера" << std::endl;
    t1.pause_continue(false);
    t2.pause_continue(false);
    t3.pause_continue(false);
    for (int i = 0; i < k; ++i)
        arX[i] -= 1;

    std::cout << "Результат замера: " << t1.vuvod() << " наносекунд"<< std::endl;
    std::cout << t2.vuvod() << " микросекунд" << std::endl;
    std::cout << t3.vuvod() << " миллисекунд" << std::endl;

    std::cout << "Продолжение замера" << std::endl;
    t1.pause_continue(false);
    t2.pause_continue(false);
    t3.pause_continue(false);
    for (int i = 0; i < k; ++i)
        arX[i] += 5;

    std::cout << "Результат замера: " << t1.vuvod() << " наносекунд" << std::endl;
    std::cout << t2.vuvod() << " микросекунд" << std::endl;
    std::cout << t3.vuvod() << " миллисекунд" << std::endl;

    std::cout << "Перезапуск таймера " << std::endl;
    t1.reset();
    t2.reset();
    t3.reset();
    for (int i = 0; i < k; ++i)
        arX[i] -= 3;

    std::cout << "Результат замера: " << t1.vuvod() << " наносекунд" << std::endl;
    std::cout << t2.vuvod() << " микросекунд" << std::endl;
    std::cout << t3.vuvod() << " миллисекунд" << std::endl;
}

#include "ThreadPool.h"

void ThreadPool::push(std::function<void(Log& event)> func, Log& event) {
    std::unique_lock<std::mutex> lock(m_lock);
    m_function_queue.push({func, event});
    lock.unlock();
    m_data_condition.notify_one();
}

void ThreadPool::done() {
    std::unique_lock<std::mutex> lock(m_lock);
    m_accept_functions = false;
    lock.unlock();
    m_data_condition.notify_all();
}

void ThreadPool::infinite_loop_func() {
    std::function<void(Log& event)> func;
    while (true) {
            std::unique_lock<std::mutex> lock(m_lock);
            m_data_condition.wait(lock, [this]() {return !m_function_queue.empty() || !m_accept_functions; });
            if (!m_accept_functions && m_function_queue.empty()) {
                return;
            }
            func = m_function_queue.front().first;
            Log& event =  m_function_queue.front().second;

            m_function_queue.pop();
        func(event);
    }
}
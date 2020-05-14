#pragma once
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cassert>
#include "LogParser.h"

class ThreadPool {
public:
    void push(std::function<void(Log& event)> func, Log& event);
    void done();
    void infinite_loop_func();

private:
    std::queue<std::pair<std::function<void(Log& event)>, Log&>> m_function_queue;
    std::mutex m_lock;
    std::condition_variable m_data_condition;
    std::atomic<bool> m_accept_functions = true;
};


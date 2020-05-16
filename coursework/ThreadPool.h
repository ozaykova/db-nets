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
    void infiniteWaiting();

private:
    std::queue<std::pair<std::function<void(Log& event)>, Log&>> q;
    std::mutex lockMemory;
    std::condition_variable dataCondition;
    std::atomic<bool> canOccur = true;
};
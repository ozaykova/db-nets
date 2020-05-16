#include "ThreadPool.h"

void ThreadPool::push(std::function<void(Log& event)> func, Log& event) {
    std::unique_lock<std::mutex> lock(lockMemory);
    q.push({func, event});
    lock.unlock();
    dataCondition.notify_one();
}

void ThreadPool::done() {
    std::unique_lock<std::mutex> lock(lockMemory);
    canOccur = false;
    lock.unlock();
    dataCondition.notify_all();
}

void ThreadPool::infiniteWaiting() {
    std::function<void(Log& event)> func;
    while (true) {
        std::unique_lock<std::mutex> lock(lockMemory);
        dataCondition.wait(lock, [this]() {return !q.empty() || !canOccur; });
        if (!canOccur && q.empty()) {
            return;
        }
        func = q.front().first;
        Log& event =  q.front().second;

        q.pop();
        func(event);
    }
}
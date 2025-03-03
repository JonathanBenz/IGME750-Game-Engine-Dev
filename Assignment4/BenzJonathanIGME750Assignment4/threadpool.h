#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
private:

    void worker(); // Worker function that each thread in the pool executes
    std::vector<std::thread> workers;  // The threads in the pool
    std::queue<std::function<void()>> tasks;  // Queue for tasks
    std::mutex queueMutex;  // Mutex to protect the task queue
    std::condition_variable condition;  // To notify threads when tasks are available
    bool stop;  // Flag to indicate whether the pool should stop accepting tasks

public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    void enqueue(std::function<void()> task);
};

#endif
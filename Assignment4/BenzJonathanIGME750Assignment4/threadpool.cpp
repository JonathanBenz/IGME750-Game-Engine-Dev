#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    // Create and start worker threads
    for (size_t i = 0; i < numThreads; ++i) {
        workers.push_back(std::thread(&ThreadPool::worker, this));
    }
}

ThreadPool::~ThreadPool() {
    // Stop all threads
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    // Notify all threads to stop
    condition.notify_all();

    // Join all threads
    for (std::thread& worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(task);
    }
    condition.notify_one();  // Notify one worker thread that a task is available
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;

        {
            // Lock the queue and wait for a task or for stop flag
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return !tasks.empty() || stop; });

            // If stop flag is set and queue is empty, exit the worker thread
            if (stop && tasks.empty()) {
                return;
            }

            // Get the task from the queue
            task = tasks.front();
            tasks.pop();
        }

        // Execute the task
        task();
    }
}
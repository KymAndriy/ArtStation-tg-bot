#include <functional>
#include <queue>
#include <chrono>
#include <mutex>
#include <future>

#include "thread_pool.h"

thread_pool::thread_pool(uint32_t num_threads)
{
    threads.reserve(num_threads);
    for (uint32_t i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(&thread_pool::run, this);
    }
}

void thread_pool::wait(size_t task_id)
{
    std::unique_lock<std::mutex> lock(completed_task_ids_mtx);
    completed_task_ids_cv.wait(lock, [this, task_id]() -> bool
                               { return completed_task_ids.find(task_id) != completed_task_ids.end(); });
}

void thread_pool::wait_all()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    completed_task_ids_cv.wait(lock, [this]() -> bool
                               {
            std::lock_guard<std::mutex> task_lock(completed_task_ids_mtx);
            return tasks_queue.empty() && last_idx == completed_task_ids.size(); });
}

void thread_pool::wait_all(std::atomic_bool &flag)
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    completed_task_ids_cv.wait(lock, [this]() -> bool
                               {
            std::lock_guard<std::mutex> task_lock(completed_task_ids_mtx);
            return tasks_queue.empty() && last_idx == completed_task_ids.size(); });
    flag = true;
}

bool thread_pool::calculated(size_t task_id)
{
    std::lock_guard<std::mutex> lock(completed_task_ids_mtx);
    if (completed_task_ids.find(task_id) != completed_task_ids.end())
    {
        return true;
    }
    return false;
}

thread_pool::~thread_pool()
{
    quit = true;
    for (uint32_t i = 0; i < threads.size(); ++i)
    {
        q_cv.notify_all();
        threads[i].join();
    }
}

void thread_pool::run()
{
    while (!quit)
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        q_cv.wait(lock, [this]() -> bool
                  { return !tasks_queue.empty() || quit; });

        if (!tasks_queue.empty())
        {
            auto elem = std::move(tasks_queue.front());
            tasks_queue.pop();
            lock.unlock();

            elem.first.get();

            std::lock_guard<std::mutex> lock(completed_task_ids_mtx);
            completed_task_ids.insert(elem.second);

            completed_task_ids_cv.notify_all();
        }
    }
}

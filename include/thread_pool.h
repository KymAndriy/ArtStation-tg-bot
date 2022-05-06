#ifndef IMG_BOT_THREAD_POOL_H
#define IMG_BOT_THREAD_POOL_H

#include <thread>
#include <vector>
#include <condition_variable>
#include <atomic>
#include <unordered_set>

#include <functional>
#include <queue>
#include <chrono>
#include <mutex>
#include <future>

class thread_pool
{
public:
    thread_pool(uint32_t num_threads);

    template <typename Func, typename... Args>
    size_t add_task(Func &&task_func, Args &&...args)
    {
        size_t task_idx = last_idx++;

        std::lock_guard<std::mutex> q_lock(queue_mutex);

        // typedef typename std::result_of<Func(Args...)>::type result_type;
        // std::packaged_task<result_type(Args...)> task(std::bind(task_func, std::forward<Args>(args)...));
        // std::packaged_task<Func(...Args)>(task.get_future(), std::forward<Args>(args)...) 
        tasks_queue.emplace(std::async(std::launch::deferred, task_func, std::forward<Args>(args)...), task_idx);
        // tasks_queue.emplace(task.get_future(), task_idx);
        q_cv.notify_one();
        return task_idx;
    }

    void wait(size_t task_id);

    void wait_all();

    void wait_all(std::atomic_bool &flag);

    bool calculated(size_t task_id);

    ~thread_pool();

private:
    void run();

    std::queue<std::pair<std::future<void>, size_t>> tasks_queue;
    std::mutex queue_mutex;
    std::condition_variable q_cv;

    std::unordered_set<size_t> completed_task_ids;
    std::condition_variable completed_task_ids_cv;
    std::mutex completed_task_ids_mtx;

    std::vector<std::thread> threads;

    std::atomic<bool> quit{false};
    std::atomic<size_t> last_idx = 0;
};

#endif
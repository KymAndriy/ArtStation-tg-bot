#include <curl/curl.h>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <array>
#include <queue>
#include <string>
// #include

// #include

template <typename T>
class queue_thread
{
    void push(T into_queue)
    {
        std::lock_guard<std::mutex> lock();
        collection_poll.push(into_queue);
        size_ = collection_poll.size();
    }

    T pop()
    {
        std::lock_guard<std::mutex> lock();
        T temp = collection_poll.front();
        collection_poll.pop();
        size_ = collection_poll.size();
        return temp;
    }

    bool has_elements()
    {
        std::lock_guard<std::mutex> lock();
        return collection_poll.size() > 0;
    }

    size_t size()
    {
        std::lock_guard<std::mutex> lock();
        return size_;
    }

private:
    size_t size_;
    std::queue<T> collection_poll;
    std::mutex adding_mutex;
};

class thread_pool
{
public:
    thread_pool()
    {
        thread_number = 4;
        for (int i = 0; i < thread_number; i++)
        {
            th_pool[i] = std::thread(thread_pool::run, this);
        }
    }

    thread_pool(size_t thread_quantity)
    {
        thread_number = (thread_quantity > 8) ? 8 : thread_quantity;
        for (int i = 0; i < thread_number; i++)
        {
            th_pool[i] = std::thread(thread_pool::run, this);
        }
    }

    void add_task(std::string url)
    {
        urls_queue.push(url);
    }

    ~thread_pool()
    {
        run_flag = false;
        q_cv.notify_all();
        for (size_t i = 0; i < thread_number; ++i)
        {
            th_pool[i].join();
        }
    }

private:
   
    void run()
    {
        while (run_flag)
        {
            /* code */
        }
    }

    size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {

        std::string file_str = "file_";
        file_str.append(std::to_string(*(int *)userdata));
        file_str.append(".json");
        return size * nmemb;
    }

    //  void run() {
    //     while (!quite) {
    //         std::unique_lock<std::mutex> lock(q_mtx);
    //         q_cv.wait(lock, [this]()->bool { return !q.empty() || quite; });

    //         if (!q.empty() && !quite) {
    //             std::pair<Task, uint64_t> task = std::move(q.front());
    //             q.pop();
    //             lock.unlock();

    //             task.first();

    //             std::lock_guard<std::mutex> lock(tasks_info_mtx);
    //             if (task.first.has_result()) {
    //                 tasks_info[task.second].result = task.first.get_result();
    //             }
    //             tasks_info[task.second].status = TaskStatus::completed;
    //             ++cnt_completed_tasks;
    //         }
    //         wait_all_cv.notify_all();
    //         tasks_info_cv.notify_all(); // notify for wait function
    //     }
    // }

    inline static const size_t array_size = 8;
    size_t thread_number;
    std::array<std::thread, array_size> th_pool;
    std::queue<std::string> urls_queue;
    std::atomic_bool run_flag = true;
};

int main()
{
    curl_global_init(CURL_GLOBAL_ALL);
}
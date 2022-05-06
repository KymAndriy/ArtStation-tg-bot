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

// template <typename T>
// class queue_thread
// {
//     queue_thread(queue_thread&) = delete; 
//     queue_thread(const queue_thread&) = delete; 
    
//     void push(T into_queue)
//     {
//         std::lock_guard<std::mutex> lock();
//         collection_poll.push(into_queue);
//         size_ = collection_poll.size();
//     }

//     T pop()
//     {
//         std::lock_guard<std::mutex> lock();
//         T temp = collection_poll.front();
//         collection_poll.pop();
//         size_ = collection_poll.size();
//         return temp;
//     }

//     bool has_elements()
//     {
//         std::lock_guard<std::mutex> lock();
//         return collection_poll.size() > 0;
//     }

//     size_t size()
//     {
//         std::lock_guard<std::mutex> lock();
//         return size_;
//     }

// private:
//     size_t size_;
//     std::queue<T> collection_poll;
//     std::mutex adding_mutex;
// };


class synchronized_queue
{
public:
    void push(int val)
    {
        // q.load().
    }


private:
    std::atomic<std::array<int, 20>> q;

};


int main()
{
  



}
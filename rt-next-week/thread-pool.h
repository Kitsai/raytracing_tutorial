#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
class thread_pool {

  public:
    thread_pool(size_t num_threads = std::thread::hardware_concurrency()) { start(num_threads); }

    void add_job(std::function<void()> job) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            jobs.push(std::move(job));
        }
        queue_cv.notify_one();
    }

    void wait_for_completion() {
        std::unique_lock<std::mutex> lock(complete_mutex);
        complete_cv.wait(lock, [this] { return active == 0 && jobs.empty(); });
    }
    ~thread_pool() { end(); }

    void end() {
        stop = true;
        queue_cv.notify_all();
        for(auto& worker : workers)
            if(worker.joinable())
                worker.join();
    }

    void start(size_t num_threads = std::thread::hardware_concurrency()) {
        workers.clear();
        for(size_t i = 0; i < num_threads; i++) {
            workers.emplace_back([this] {
                while(true) {
                    std::function<void()> job;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        queue_cv.wait(lock, [this] { return !jobs.empty() || stop; });
                        if(stop && jobs.empty())
                            return;
                        job = std::move(jobs.front());
                        jobs.pop();
                        active++;
                    }
                    job();
                    {
                        std::lock_guard<std::mutex> lock(complete_mutex);
                        active--;
                        complete_cv.notify_one();
                    }
                }
            });
        }
    }

  private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;

    std::mutex queue_mutex, complete_mutex;
    std::condition_variable queue_cv, complete_cv;

    std::atomic<size_t> active{0};
    std::atomic<bool> stop{false};
};

#pragma once

#include <mutex>
#include <queue>

namespace ntd {
    /**
     * @brief thread safe queue
     * is not safe if more than one thread accesses the top of the queue. in that case, the top of the queue
     * must be thread safe.
     * 
     */
    template <typename T>
    class ThreadQueue {
        std::mutex lock;
        std::queue<T> internal;
    public:
        /**
         * @brief push to the queue
         * 
         * @param x obj
         */
        void Push(T& x) {
            std::lock_guard<std::mutex> temp(lock);
            internal.push(x);
        }
        
        /**
         * @brief get size of q
         * 
         * @return int 
         */
        int Size() {
            std::lock_guard<std::mutex> temp(lock);
            return this->internal.size();
        }

        /**
         * @brief pop the front of the queue
         * 
         */
        void Pop() {
            std::lock_guard<std::mutex> temp(lock);
            if (this->internal.empty()) {
                throw std::runtime_error("queue was empty when pop was called");
            }
            internal.pop();
        }

        /**
         * @brief give the reference to the front of the queue
         * 
         * @return T& the reference to the front of the queue
         */
        T& Front() {
            std::lock_guard<std::mutex> temp(lock);
            if (this->internal.empty()) {
                throw std::runtime_error("queue was empty when front was called");
            }
            return internal.front();
        }

        /**
         * @brief return if queue is empty
         * 
         * @return true 
         * @return false 
         */
        bool Empty() {
            std::lock_guard<std::mutex> temp(lock);
            return internal.empty();
        }

        /**
         * @brief emplace
         * 
         * @tparam Args 
         * @param args 
         */
        template <typename... Args>
        void Emplace(Args&&... args) {
            std::lock_guard<std::mutex> temp(lock);
            this->internal.push(T(args...));
        }
    };
}
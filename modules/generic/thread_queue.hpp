#pragma once

#include <queue>
#include <shared_mutex>
#include <mutex>

namespace ntd {
    /**
     * @brief thread safe queue
     * is not safe if more than one thread accesses the top of the queue. in that case, the top of the queue
     * must be thread safe.
     * 
     */
    template <typename T>
    class ThreadQueue {
        std::shared_mutex mutex;
        std::queue<T> internal;
    public:
        /**
         * @brief push to the queue
         * 
         * @param x obj
         */
        void Push(T& x) {
            std::scoped_lock lock{this->mutex};
            internal.push(x);
        }

        /**
         * @brief push to the queue
         * 
         * @param x obj
         */
        void Push(T&& x) {
            std::scoped_lock lock{this->mutex};
            internal.push(x);
        }
        
        /**
         * @brief get size of q
         * 
         * @return int 
         */
        int Size() {
            std::shared_lock lock{this->mutex};
            return this->internal.size();
        }

        /**
         * @brief pop the front of the queue
         * 
         */
        void Pop() {
            std::scoped_lock lock{this->mutex};
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
            std::shared_lock lock{this->mutex};
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
            std::shared_lock lock{this->mutex};
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
            std::scoped_lock lock{this->mutex};
            this->internal.push(T(args...));
        }
    };
}
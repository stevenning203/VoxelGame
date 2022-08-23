#pragma once
#include <unordered_map>
#include <queue>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mutex>
#include <generic/workable.hpp>
#include <generic/input_observer.hpp>
#include <generic/input_subject.hpp>

namespace Project {
    class Display;

    /**
     * @brief A key handler that handles input.
     * 
     */
    class KeyHandler : public Workable, public InputObserver, public InputSubject {
        /**
         * @brief release all held keys
         * 
         */
        void Update();

    public:
        /**
         * @brief Construct a new Key Handler objectd
         * 
         * @param d the display that this listener will be added to
         */
        KeyHandler();

        virtual void Notify(const Input& input) override;

        /**
         * @brief get the state of the given key
         * 
         * @param key the GLFW_KEY that is desired
         * @param type 4 is KEY_DOWN, 2 is KEY_UP, and 1 is KEY_HELD
         * @return true the state is true...
         * @return false the state is false...
         */
        bool GetKeyState(int key, int type);

        virtual void MainThreadWork() override;

        virtual void ThreadWork() override;
        
    };
}
#pragma once
#include <unordered_map>
#include <queue>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mutex>
#include <generic/workable.hpp>

namespace Project {
    class Display;

    /**
     * @brief A key handler that handles input.
     * 
     */
    class KeyHandler : public Workable {
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

    void KeyHandlerGLFWCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}
#pragma once
#include <unordered_map>
#include <queue>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mutex>

extern std::unordered_map<int, int> keyboard_state;
extern std::queue<int> key_reset_queue;
extern std::mutex key_mutex;

namespace Project {
    class Display;

    /**
     * @brief A key handler that handles input.
     * 
     */
    class KeyHandler {
    public:
        /**
         * @brief Construct a new Key Handler objectd
         * 
         * @param d the display that this listener will be added to
         */
        KeyHandler(Display& d);

        /**
         * @brief get the state of the given key
         * 
         * @param key the GLFW_KEY that is desired
         * @param type 4 is KEY_DOWN, 2 is KEY_UP, and 1 is KEY_HELD
         * @return true the state is true...
         * @return false the state is false...
         */
        bool GetKeyState(int key, int type);

        /**
         * @brief release all held keys
         * 
         */
        void Update();
    };

    void KeyHandlerGLFWCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}
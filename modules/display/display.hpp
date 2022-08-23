#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <atomic>
#include <generic/input_subject.hpp>

namespace Project {
    class Program;
    class KeyHandler;
    class MouseHandler;
    
    /**
     * @brief a frame buffer callback that is called when the display is in some way modified
     * 
     * @param w 
     * @param width 
     * @param height 
     */
    void FrameBufferCallback(GLFWwindow* w, int width, int height);

    /**
     * @brief mouse button callback for cpp
     * 
     * @param w window
     * @param b button
     * @param a action
     * @param m mode
     */
    void MouseHandlerGLFWCallback(GLFWwindow* w, int b, int a, int m);

    /**
     * @brief keyboard callback
     * 
     * @param window
     * @param key 
     * @param scancode 
     * @param action 
     * @param mods 
     */
    void KeyHandlerGLFWCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /**
     * @brief A display for the project. Initializes with a given window width and window height.
     * Calls window hints and suggests a version of 330. If the system is defined by __APPLE__ then it
     * should suggest forward compatibility.
     */
    class Display : public InputSubject {
    private:
        static constexpr float INITIAL_FOV = 3.1415926f / 2.f;
        int width, height;
        GLFWwindow* pointer;
        glm::mat4 projection_matrix;
        glm::mat4 orthographic_matrix;
        Program* shader;
        std::atomic<bool> should_close;

        Display();

        ~Display() = default;

        Display(const Display&) = delete;

        Display& operator=(const Display&) = delete;
        
    public:
        static Display& GetInstance();

        static constexpr int INITIAL_FPS = 144;

        /**
         * @brief Construct a new Display object and initialize glfw and glad.
         * 
         * @param width the width of the display to be initialized
         * @param height the height of the display to be initialized
         * @param title the title of the window; default is 'GLFW Window'
         */
        void Init(const int width, const int height, const char* title = "GLFW Window");

        /**
         * @brief attach the given mouse handler to this display
         * 
         * @param mouse 
         */
        void AttachMouseHandler(const MouseHandler& mouse);

        /**
         * @brief attach the given key handler to this display
         * 
         * @param keyboard 
         */
        void AttackKeyboardHandler(const KeyHandler& keyboard);

        /**
         * @brief hint a mouse input from the callback
         * 
         * @param b 
         * @param a 
         * @param m 
         */
        void HintMouseInput(const int b, const int a, const int m);

        /**
         * @brief hint a keyboard input from the callback
         * 
         * @param key 
         * @param scancode 
         * @param action 
         * @param mods 
         */
        void HintKeyboardInput(const int key, const int scancode, const int action, const int mods);

        /**
         * @brief Set the Shader object
         * 
         * @param shader the shader
         */
        void SetShader(Program* shader);

        /**
         * @brief Swap the front and back buffers of this display.
         * 
         */
        void SwapBuffers();

        /**
         * @brief push the projection matrix
         * 
         */
        void PushMatrix();

        /**
         * @brief push ortho
         * 
         */
        void PushOrtho();

        /**
         * @brief return whether or not the window should close.
         * 
         * @return true the window should close
         * @return false the window should not close
         */
        bool ShouldClose();

        /**
         * @brief hint that the display should close
         * 
         */
        void HintClose();

        GLFWwindow* GetWindowPointer();

        /**
         * @brief update the display to the width and height specified.
         * 
         */
        void SuggestDimensions();

        int GetWidth();

        int GetHeight();
    };
}
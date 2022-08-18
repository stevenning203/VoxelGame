#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <atomic>

namespace Project {
    class Program;
    
    void FrameBufferCallback(GLFWwindow* w, int width, int height);
    /**
     * @brief A display for the project. Initializes with a given window width and window height.
     * Calls window hints and suggests a version of 330. If the system is defined by __APPLE__ then it
     * should suggest forward compatibility.
     */
    class Display {
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
#pragma once

#include <glm/glm.hpp>
#include <mutex>
#include <shared_mutex>
#include <generic/workable.hpp>

namespace Project {
    class Program;
    class MouseHandler;
    class Timer;
    class KeyHandler;
    class Program;

    /**
     * @brief A camera inside the game.
     * 
     */
    class Camera : public Workable {
        static constexpr float INITIAL_SENSITIVITY = 14.f;
        static constexpr float INITIAL_SPEED = 7.75f;
        float yaw;
        float pitch;
        float sensitivity;
        float speed;
        glm::mat4 matrix;
        glm::vec3 forward, position;
        glm::vec3 up;
        glm::vec3 right;
        bool changed;
        std::shared_mutex mutex;
        Program* shader;
        MouseHandler* mouse;
        KeyHandler* keyboard;
        Timer* timer;

        /**
         * @brief update the camera based on the mouse dx and dy
         * 
         * @param m the mouse input handler
         * @param t the timer object
         */
        void UpdatePanning();

        /**
         * @brief update the camera based on the keyboard keys held
         * 
         * @param keyboard the keyboard handler
         * @param t the timer object
         */
        void UpdateMovement();

        /**
         * @brief push the matrix into the shader program given; compute it if something changed.
         * 
         * @param shader the shader to be uniformed
         */
        void PushMatrix();
    public:
        /**
         * @brief Construct a new Camera object
         * 
         */
        Camera(Program* shader, MouseHandler* mouse, KeyHandler* kb, Timer* timer);

        virtual void MainThreadWork() override;
        
        virtual void ThreadWork() override;

        const glm::vec3& GetForward() const;

        const glm::vec3& GetPosition() const;

        const glm::vec3& GetRight() const;
    };
}
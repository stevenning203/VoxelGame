#pragma once

#include <glm/glm.hpp>
#include <generic/positionable.hpp>

namespace Project {
    class Program;
    class MouseHandler;
    class Timer;
    class KeyHandler;

    /**
     * @brief A camera inside the game.
     * 
     */
    class Camera : public Positionable {
        static constexpr float INITIAL_SENSITIVITY = 7.5f;
        static constexpr float INITIAL_SPEED = 3.75f;
        float yaw;
        float pitch;
        float sensitivity;
        float speed;
        glm::mat4 matrix;
        glm::vec3 forward;
        glm::vec3 up;
        glm::vec3 right;
        bool changed;
    public:
        /**
         * @brief Construct a new Camera object
         * 
         */
        Camera();

        /**
         * @brief push the matrix into the shader program given; compute it if something changed.
         * 
         * @param shader the shader to be uniformed
         */
        void PushMatrix(Program& shader);

        /**
         * @brief update the camera based on the mouse dx and dy
         * 
         * @param m the mouse input handler
         * @param t the timer object
         */
        void UpdatePanning(MouseHandler& m, Timer& t);

        /**
         * @brief update the camera based on the keyboard keys held
         * 
         * @param keyboard the keyboard handler
         * @param t the timer object
         */
        void UpdateMovement(KeyHandler& keyboard, Timer& t);

        const glm::vec3& GetForward() const;

        const glm::mat4& GetMatrix() const;
    };
}
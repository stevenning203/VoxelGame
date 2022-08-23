#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <atomic>
#include <generic/workable.hpp>
#include <generic/input_observer.hpp>
#include <generic/input_subject.hpp>

extern std::atomic<bool> key_state[6];

namespace Project {
    class Display;

    /**
     * @brief A mouse input handler that provides input information about the mouse
     * 
     */
    class MouseHandler : public Workable, public InputObserver, public InputSubject {
    public:
        enum class MouseEnum {
            LMB_DOWN = 0, RMB_DOWN, LMB_HELD, RMB_HELD, LMB_UP, RMB_UP
        };

    private:
        std::atomic<int> mousex = 0, mousey = 0, dx = 0, dy = 0;
        std::atomic<int> lx = 0, ly = 0;

        /**
         * @brief update the mouse states
         * 
         * @param display the active display that you want to get the mouse position and states for.
         */
        void Update();

    public:

        /**
         * @brief Construct a new Mouse object and add a callback for mouse input to the glfw window.
         * 
         */
        MouseHandler();

        virtual void Notify(const Input& input) override;

        /**
         * @brief Get the mouse _state of the given button
         * 
         * @param enumer the enum for the mouse button
         * @return true the button is true
         * @return false the _state is false
         */
        bool GetMouseState(MouseEnum enumer) const;

        virtual void MainThreadWork() override;

        virtual void ThreadWork() override;

        int MouseX() const;

        int MouseDX() const;

        int MouseDY() const;

        int MouseY() const;
    };
}
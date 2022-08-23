#pragma once

#include <vector>
#include <glm/vec2.hpp>

namespace Project {
    class InputObserver;

    class Input {
        public:
            enum class InputEnum {
                EVENT_MOUSE_MOVE,
                EVENT_KEYBOARD_STROKE,
                EVENT_MOUSE_BUTTON
            };

            /**
             * @brief Construct a new Input object
             * 
             * @param type type of input that occured
             * @param code the id of the input event
             * @param action the action done with the code
             * @param x position of the input, x
             * @param y position of the input, y
             */
            Input(InputEnum type, const int code, const int action, const int x, const int y);

            /**
             * @brief Construct a new Input object
             * 
             * @param type type of input that occured
             * @param code the id of the input event
             * @param action the action of the code
             */
            Input(InputEnum type, const int code, const int action);

            InputEnum GetType() const;

            const glm::ivec2& GetPos() const;

            int GetCode() const;

            int GetAction() const;

        private:
            InputEnum type;
            glm::ivec2 pos;
            int code, action;
        };

    /**
     * @brief observable object that notifies observers when an input event occurs;
     * 
     */
    class InputSubject {
    public:
        /**
         * @brief subscribe the observer to this subject
         * 
         * @param observer 
         */
        void Subscribe(InputObserver& observer);

    protected:
        /**
         * @brief notify observers
         * 
         * @param input the input
         * 
         */
        virtual void NotifyObservers(const Input& input);

    private:
        std::vector<InputObserver*> observers;
    };
}
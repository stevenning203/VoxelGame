#pragma once

namespace Project {
    /**
     * @brief A class intended to be inherited. Contains an id and an ability to retrieve the id but not set it.
     * 
     * @tparam T the type of the id
     */
    template<typename T>
    class Identifiable {
    protected:
        T id;
    public:
        T GetID() {
            return this->id;
        }
    };
}

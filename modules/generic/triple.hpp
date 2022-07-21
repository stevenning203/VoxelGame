#pragma once

namespace ntd {
    /**
     * @brief triplet
     * 
     */
    template <typename T0, typename T1, typename T2>
    class Triple {
        T0 first;
        T1 second;
        T2 third;
    public:
        Triple(T0& a, T1& b, T2& c) {
            first = a;
            second = b;
            third = c;
        }

        T0 First() {
            return first;
        }
        
        T1 Second() {
            return second;
        }

        T2 Third() {
            return third;
        }
    };

    /**
     * @brief quad
     * 
     */
    template <typename T0, typename T1, typename T2, typename T3>
    class Quadlet {
        T0 first;
        T1 second;
        T2 third;
        T3 fourth;
    public:
        Quadlet(const T0& a, const T1& b, const T2& c, const T3& d) {
            first = a;
            second = b;
            third = c;
            fourth = d;
        }

        T0 First() {
            return first;
        }
        
        T1 Second() {
            return second;
        }

        T2 Third() {
            return third;
        }

        T3 Fourth() {
            return fourth;
        }
    };
}
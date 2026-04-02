#ifndef PHYSICS_TPP
#define PHYSICS_TPP

#include "physics/Physics.h"

namespace physics {

    // ~~~~~~~~~~~~~~~~~ Derivative ~~~~~~~~~~~~~~~~~

    template <typename T, core::size_type D>
    Derivative<T, D>& Derivative<T, D>::operator+=(const Derivative& other) {
        d_position += other.d_position;
        d_velocity += other.d_velocity;
        return *this;
    }

    template <typename T, core::size_type D>
    Derivative<T, D>& Derivative<T, D>::operator*=(T scalar) {
        d_position *= scalar;
        d_velocity *= scalar;
        return *this;
    }

    // ~~~~~~~~~~~~~~~~~ State ~~~~~~~~~~~~~~~~~

    template <typename T, core::size_type D>
    State<T, D>& State<T, D>::operator+=(const Derivative<T, D>& d) {
        position += d.d_position;
        velocity += d.d_velocity;
        return *this;
    }

    // ~~~~~~~~~~~~~~~~~ Global Operators ~~~~~~~~~~~~~~~~~

    template <typename T, core::size_type D>
    Derivative<T, D> operator+(Derivative<T, D> lhs, const Derivative<T, D>& rhs) {
        return lhs += rhs;
    }

    template <typename T, core::size_type D>
    Derivative<T, D> operator*(Derivative<T, D> d, T scalar) {
        return d *= scalar;
    }

    template <typename T, core::size_type D>
    Derivative<T, D> operator*(T scalar, Derivative<T, D> d) {
        return d *= scalar;
    }

    template <typename T, core::size_type D>
    State<T, D> operator+(State<T, D> s, const Derivative<T, D>& d) {
        return s += d;
    }

} // namespace physics

#endif // PHYSICS_TPP

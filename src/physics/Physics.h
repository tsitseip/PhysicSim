/**
 * @file Physics.h
 * @brief Physics structures and operators for the simulation.
 */

#ifndef PHYSICS_H
#define PHYSICS_H

#include "core/Globals.h"
#include "math/Math.h"

/**
 * @namespace physics
 * @brief Namespace for physics-related structures and operations.
 */
namespace physics {

    /**
     * @brief Represents the derivative of a state (position and velocity).
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    struct Derivative {
        math::Vector<T, D> d_position; /**< The rate of change of position. */
        math::Vector<T, D> d_velocity; /**< The rate of change of velocity. */

        /**
         * @brief Compound addition operator for derivatives.
         * @param other The other derivative.
         * @return Reference to this derivative.
         */
        Derivative& operator+=(const Derivative& other);

        /**
         * @brief Compound scalar multiplication operator for derivatives.
         * @param scalar The scalar.
         * @return Reference to this derivative.
         */
        Derivative& operator*=(T scalar);
    };

    /**
     * @brief Represents the state of a physical body (position and velocity).
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    struct State {
        math::Vector<T, D> position; /**< The position vector. */
        math::Vector<T, D> velocity; /**< The velocity vector. */

        /**
         * @brief Compound addition operator for updating state with a derivative.
         * @param d The derivative.
         * @return Reference to this state.
         */
        State& operator+=(const Derivative<T, D>& d);
    };

    /**
     * @brief Represents a physical body in the simulation.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template<typename T, core::size_type D>
    struct Body {
        core::string name; /**< The name of the body. */
        State<T, D> state; /**< The current state of the body. */
        T mass; /**< The mass of the body. */
        T radius; /**< The radius of the body. */
        bool active = true; /**< Whether the body is active in the simulation. */

        core::list<math::Vector<T, D>> trail; /**< The body's trail history. */
        bool trailEnabled = false; /**< Whether trailing is enabled for this body. */
        core::size_type maxTrailSize = 100; /**< The maximum number of points in the trail. */

        math::Vector<float, 3> color; /**< The color of the body (for rendering). */

        /**
         * @brief Construct a new Body object.
         */
        Body() : trail(core::list<math::Vector<T, D>>()) {}
    };

    /**
     * @brief Derivative addition operator.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     * @param lhs The left-hand side derivative.
     * @param rhs The right-hand side derivative.
     * @return The resulting derivative.
     */
    template <typename T, core::size_type D>
    Derivative<T, D> operator+(Derivative<T, D> lhs, const Derivative<T, D>& rhs);

    /**
     * @brief Derivative-scalar multiplication operator.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     * @param d The derivative.
     * @param scalar The scalar.
     * @return The resulting derivative.
     */
    template <typename T, core::size_type D>
    Derivative<T, D> operator*(Derivative<T, D> d, T scalar);

    /**
     * @brief Scalar-derivative multiplication operator.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     * @param scalar The scalar.
     * @param d The derivative.
     * @return The resulting derivative.
     */
    template <typename T, core::size_type D>
    Derivative<T, D> operator*(T scalar, Derivative<T, D> d);

    /**
     * @brief State-derivative addition operator.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     * @param s The state.
     * @param d The derivative.
     * @return The resulting state.
     */
    template <typename T, core::size_type D>
    State<T, D> operator+(State<T, D> s, const Derivative<T, D>& d);

} // namespace physics

#include "Physics.tpp"

#endif // PHYSICS_H

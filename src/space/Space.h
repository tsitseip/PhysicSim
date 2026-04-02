/**
 * @file Space.h
 * @brief Interface for spatial models and acceleration calculations.
 */

#ifndef SPACE_H
#define SPACE_H

#include "core/Globals.h"
#include "physics/Physics.h"

/**
 * @namespace space
 * @brief Namespace for spatial models and topology.
 */
namespace space {

    /**
     * @brief Interface for a spatial model in the simulation.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, std::size_t D>
    class ISpace {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~ISpace() = default;
        
        /**
         * @brief Computes the acceleration of a body.
         * @param bodyIndex The index of the body in the bodies array.
         * @param current_states The current states of all bodies.
         * @param bodies The physical properties of all bodies.
         * @return The computed acceleration vector.
         */
        virtual math::Vector<T, D> getAcceleration(
            size_t bodyIndex, 
            const core::array<physics::State<T, D>>& current_states,
            const core::array<physics::Body<T, D>>& bodies) const = 0;

        /**
         * @brief Applies spatial topology (e.g., boundary conditions) to a state.
         * @param state The state to modify.
         */
        virtual void applyTopology(physics::State<T, D>& state) const = 0;
    };

} // namespace space

#endif // SPACE_H

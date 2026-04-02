/**
 * @file Newtonian.h
 * @brief Implements the GravitySpace class which simulates Newtonian gravity in a space environment.
 */
#ifndef NEWTONIAN_H
#define NEWTONIAN_H

#include "space/Space.h"
#include "core/Registry.h"

/**
 * @namespace space::newtonian
 * @brief Contains implementations of space environments based on Newtonian physics.
 */
namespace space::newtonian {

    /**
     * @brief A space environment that simulates Newtonian gravity.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class GravitySpace : public space::ISpace<T, D> {
        T m_G; ///< Gravitational constant.
        T m_softening; ///< Softening parameter to prevent singularities at zero distance.
    public:
        /**
         * @brief Constructs a GravitySpace.
         * @param G Gravitational constant.
         * @param softening Softening parameter.
         */
        GravitySpace(T G, T softening) : m_G(G), m_softening(softening) {}
        ~GravitySpace() = default;

        /**
         * @brief Calculates the acceleration of a body due to Newtonian gravity.
         * @param bodyIndex Index of the body to calculate acceleration for.
         * @param current_states Array of current states of all bodies.
         * @param bodies Array of all bodies in the simulation.
         * @return The calculated acceleration vector.
         */
        math::Vector<T, D> getAcceleration(
            core::size_type bodyIndex, 
            const core::array<physics::State<T, D>>& current_states,
            const core::array<physics::Body<T, D>>& bodies) const override {
            
            math::Vector<T, D> accel{};
            const auto& my_pos = current_states[bodyIndex].position;

            for (core::size_type i = 0; i < bodies.size(); ++i) {
                if (i == bodyIndex || !bodies[i].active) continue;
                
                math::Vector<T, D> r_vec = current_states[i].position - my_pos;
                T r_sq = math::dot(r_vec, r_vec) + m_softening;
                T r = math::sqrt(r_sq);
                
                T r_power_D = math::pow(r, static_cast<T>(D)); 
                accel += r_vec * (m_G * bodies[i].mass / r_power_D);
            }
            return accel;
        }
        
        void applyTopology(physics::State<T, D>& state) const override {}
    };

} // namespace space::newtonian

#endif

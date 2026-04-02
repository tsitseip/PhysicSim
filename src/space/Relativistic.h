/**
 * @file Relativistic.h
 * @brief Defines the EinsteinSpace class which implements relativistic gravitational interactions using the Einstein-Infeld-Hoffmann equations.
 */
#ifndef RELATIVISTIC_H
#define RELATIVISTIC_H

#include "space/Space.h"
#include "core/Registry.h"

/**
 * @namespace space::relativistic
 * @brief Contains implementations of space environments based on relativistic physics.
 */
namespace space::relativistic {

    /**
     * @brief Einstein-Infeld-Hoffmann relativistic gravity space.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class EinsteinSpace : public space::ISpace<T, D> {
        T m_G; ///< Gravitational constant.
        T m_c; ///< Speed of light.
        T m_softening; ///< Softening parameter to prevent singularities.
    public:
        /**
         * @brief Constructs an EinsteinSpace.
         * @param G Gravitational constant.
         * @param c Speed of light.
         * @param softening Softening parameter.
         */
        EinsteinSpace(T G, T c, T softening)                                                                                                                                      
            : m_G(G), m_c(c), m_softening(softening) {} 
        
        /**
         * @brief Calculates the acceleration of a body using the Einstein-Infeld-Hoffmann equations.
         * @param bodyIndex Index of the body to calculate acceleration for.
         * @param current_states Array of current states of all bodies.
         * @param bodies Array of all bodies in the simulation.
         * @return The calculated relativistic acceleration vector.
         */
        math::Vector<T, D> getAcceleration(                                                                                                                                                            
            core::size_type bodyIndex,                                                                                                                                                                     
            const core::array<physics::State<T, D>>& current_states,                                                                                                                                   
            const core::array<physics::Body<T, D>>& bodies) const override {                                                                                                                           
    
            core::size_type n = bodies.size();                                                                                                                                                             
            math::Vector<T, D> accel{};                                                                                                                                                               
            T c2 = m_c * m_c;                                                                                                                                                                          
                                                                                                                                                                                                       
            const auto& pos_i = current_states[bodyIndex].position;                                                                                                                                    
            const auto& vel_i = current_states[bodyIndex].velocity;                                                                                                                                    
            T v_i_sq = math::dot(vel_i, vel_i);                                                                                                                                                        
                                                                                                                                                                                                       
            // 1. Calculate the gravitational potential U at the point of body i                                                                                                                       
            T U_i = 0.0;                                                                                                                                                                               
            for (core::size_type k = 0; k < n; ++k) {                                                                                                                                                      
                if (k == bodyIndex || !bodies[k].active) continue;                                                                                                                                     
                math::Vector<T, D> r_ik_vec = current_states[k].position - pos_i;                                                                                                                      
                T r_ik = math::sqrt(math::dot(r_ik_vec, r_ik_vec) + m_softening);                                                                                                                      
                U_i += (m_G * bodies[k].mass) / r_ik;                                                                                                                                                  
            }                                                                                                                                                                                          
                                                                                                                                                                                                       
            // 2. Main EIH interaction loop                                                                                                                                                            
            for (core::size_type j = 0; j < n; ++j) {                                                                                                                                                      
                if (j == bodyIndex || !bodies[j].active) continue;                                                                                                                                     
                                                                                                                                                                                                       
                const auto& pos_j = current_states[j].position;                                                                                                                                        
                const auto& vel_j = current_states[j].velocity;                                                                                                                                        
                                                                                                                                                                                                       
                math::Vector<T, D> r_ij_vec = pos_j - pos_i;                                                                                                                                           
                T r_ij_sq = math::dot(r_ij_vec, r_ij_vec) + m_softening;                                                                                                                               
                T r_ij = math::sqrt(r_ij_sq);                                                                                                                                                          
                T r_ij_cubed = r_ij_sq * r_ij;                                                                                                                                                         
                                                                                                                                                                                                       
                // Calculate potential U at j                                                                                                                                                          
                T U_j = 0.0;                                                                                                                                                                           
                for (std::size_t k = 0; k < n; ++k) {                                                                                                                                                  
                    if (k == j || !bodies[k].active) continue;                                                                                                                                         
                    math::Vector<T, D> r_jk_vec = current_states[k].position - pos_j;                                                                                                                  
                    T r_jk = math::sqrt(math::dot(r_jk_vec, r_jk_vec) + m_softening);                                                                                                                  
                    U_j += (m_G * bodies[k].mass) / r_jk;                                                                                                                                             
                }                                                                                                                                                                                      

                T v_j_sq = math::dot(vel_j, vel_j);                                                                                                                                                    
                T dot_vi_vj = math::dot(vel_i, vel_j);                                                                                                                                                 
                T dot_rij_vj = math::dot(r_ij_vec, vel_j);                                                                                                                                             
                                                                                                                                                                                                       
                T newtonian_mag = (m_G * bodies[j].mass) / r_ij_cubed;                                                                                                                                 
                                                                                                                                                                                                       
                // Relativistic correction bracket                                                                                                                                                     
                T bracket_term = core::constants::space::relativistic::EIH_DELTA
                    - (core::constants::space::relativistic::EIH_ALPHA * U_i / c2)
                    - (core::constants::space::relativistic::EIH_DELTA * U_j / c2)
                    + (v_i_sq / c2)
                    + (2.0 * v_j_sq / c2)
                    - (core::constants::space::relativistic::EIH_ALPHA * dot_vi_vj / c2)
                    - (core::constants::space::relativistic::EIH_BETA * (dot_rij_vj * dot_rij_vj) / (c2 * r_ij_sq));

                math::Vector<T, D> vel_diff = vel_i - vel_j;
                math::Vector<T, D> vel_weighted = (vel_i * core::constants::space::relativistic::EIH_ALPHA) - (vel_j * core::constants::space::relativistic::EIH_GAMMA);
                T dot_rij_vel_weighted = math::dot(r_ij_vec, vel_weighted);                                                                                                                            
                                                                                                                                                                                                       
                T velocity_term_mag = (m_G * bodies[j].mass) / (c2 * r_ij_cubed);                                                                                                                      
                                                                                                                                                                                                       
                accel += (r_ij_vec * (newtonian_mag * bracket_term))                                                                                                                                   
                       + (vel_diff * (velocity_term_mag * dot_rij_vel_weighted));                                                                                                                      
            }                                                                                                                                                                                          
            return accel;                                                                                                                                                                              
        }                                                                                                                                                                                              
                                                                                                                                                                                                       
        void applyTopology(physics::State<T, D>& state) const override {}                                                                                                                              
    };

} // namespace space::relativistic
#endif

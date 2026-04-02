#include "universe/ISimulationStep.h"
#include "core/Registry.h"
#include "core/Globals.h"

namespace universe::steps {

    /**
     * @brief Body merging simulation step based on proximity.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class MergingStep : public ISimulationStep<T, D> {
    public:
        /**
         * @brief Checks for collisions between bodies and merges them if necessary.
         * @param bodies Array of bodies in the simulation.
         * @param space The space environment (not used in this step).
         * @param dt The time step size (not used in this step).
         */
        void execute(core::array<physics::Body<T, D>>& bodies,
                             const space::ISpace<T, D>& space,
                             T dt) override {
            core::size_type n = bodies.size();
            for (core::size_type i = 0; i < n; ++i) {
                if (!bodies[i].active) continue;
                for (core::size_type j = i + 1; j < n; ++j) {
                    if (!bodies[j].active) continue;

                    math::Vector<T, D> r_vec = bodies[j].state.position - bodies[i].state.position;
                    T dist_sq = math::dot(r_vec, r_vec);
                    T rad_sum = bodies[i].radius + bodies[j].radius;

                    if (dist_sq < rad_sum * rad_sum) {
                        core::size_type heavy = (bodies[i].mass > bodies[j].mass) ? i : j;
                        core::size_type light = (heavy == i) ? j : i;

                        // Conservation of momentum: (m1*v1 + m2*v2) / (m1+m2)
                        bodies[heavy].state.velocity = 
                            (bodies[heavy].state.velocity * bodies[heavy].mass + 
                            bodies[light].state.velocity * bodies[light].mass) * 
                            (1.0 / (bodies[heavy].mass + bodies[light].mass));
                        
                        bodies[heavy].mass += bodies[light].mass;
                        
                        // Simple radius growth logic
                        bodies[heavy].radius += bodies[light].radius * core::constants::simulation::MERGE_GROWTH_FACTOR; 

                        bodies[light].active = false;
                    }
                }
            }
        }
    };

} // namespace universe::steps

static core::Registrar<universe::ISimulationStep<double, DIMENSION>> reg_merging(core::string(core::constants::universe::MERGING_STEP), [](const core::json& j) {
    return core::make_unique<universe::steps::MergingStep<double, DIMENSION>>();
});

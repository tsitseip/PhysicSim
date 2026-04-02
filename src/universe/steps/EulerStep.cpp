#include "universe/ISimulationStep.h"
#include "core/Registry.h"
#include "core/Globals.h"

namespace universe::steps {

    /**
     * @brief Euler method simulation step implementation.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class EulerStep : public ISimulationStep<T, D> {
    public:
        /**
         * @brief Executes the Euler integration step.
         * @param bodies Array of bodies in the simulation.
         * @param space The space environment providing gravitational acceleration.
         * @param dt The time step size.
         */
        void execute(core::array<physics::Body<T, D>>& bodies,
                             const space::ISpace<T, D>& space,
                             T dt) override {
            core::size_type n = bodies.size();
            core::array<physics::State<T, D>> current_states(n);
            for (core::size_type i = 0; i < n; ++i) current_states[i] = bodies[i].state;

            for (core::size_type i = 0; i < n; ++i) {
                if (!bodies[i].active) continue;
                physics::Derivative<T, D> k = { current_states[i].velocity, space.getAcceleration(i, current_states, bodies) };
                bodies[i].state += k * dt;
                space.applyTopology(bodies[i].state);
            }
        }
    };

} // namespace universe::steps

static core::Registrar<universe::ISimulationStep<double, DIMENSION>> reg_euler(core::string(core::constants::universe::EULER_STEP), [](const core::json& j) {
    return core::make_unique<universe::steps::EulerStep<double, DIMENSION>>();
});

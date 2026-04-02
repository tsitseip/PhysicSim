#include "universe/ISimulationStep.h"
#include "core/Registry.h"
#include "core/Globals.h"

namespace universe::steps {

    /**
     * @brief Runge-Kutta 4th Order simulation step implementation.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class RK4Step : public ISimulationStep<T, D> {
    public:
        /**
         * @brief Executes the 4th order Runge-Kutta integration step.
         * @param bodies Array of bodies in the simulation.
         * @param space The space environment providing gravitational acceleration.
         * @param dt The time step size.
         */
        void execute(core::array<physics::Body<T, D>>& bodies,
                             const space::ISpace<T, D>& space,
                             T dt) override {
               
            core::size_type n = bodies.size();
            core::array<physics::State<T, D>> initial_states(n);
            for (core::size_type i = 0; i < n; ++i) initial_states[i] = bodies[i].state;

            auto get_states = [&](const core::array<physics::Derivative<T, D>>& k, T dt_step) {
                core::array<physics::State<T, D>> res(n);
                for(core::size_type i = 0; i < n; ++i) res[i] = initial_states[i] + (k[i] * dt_step);
                return res;
            };

            core::array<physics::Derivative<T, D>> k1(n), k2(n), k3(n), k4(n);

            // k1 = f(t, y)
            for (core::size_type i = 0; i < n; ++i) 
                k1[i] = { initial_states[i].velocity, space.getAcceleration(i, initial_states, bodies) };

            // k2 = f(t + h/2, y + h*k1/2)
            auto s2 = get_states(k1, dt * 0.5);
            for (core::size_type i = 0; i < n; ++i) 
                k2[i] = { s2[i].velocity, space.getAcceleration(i, s2, bodies) };

            // k3 = f(t + h/2, y + h*k2/2)
            auto s3 = get_states(k2, dt * 0.5);
            for (core::size_type i = 0; i < n; ++i) 
                k3[i] = { s3[i].velocity, space.getAcceleration(i, s3, bodies) };

            // k4 = f(t + h, y + h*k3)
            auto s4 = get_states(k3, dt);
            for (core::size_type i = 0; i < n; ++i) 
                k4[i] = { s4[i].velocity, space.getAcceleration(i, s4, bodies) };

            // Final integration
            for (core::size_type i = 0; i < n; ++i) {
                if(!bodies[i].active) continue;
                
                // weighted average of slopes: (k1 + 2k2 + 2k3 + k4) / 6
                physics::Derivative<T, D> final_k = (k1[i] + (k2[i] + k3[i]) * 2.0 + k4[i]) * (1.0 / 6.0);
                bodies[i].state += final_k * dt;
                space.applyTopology(bodies[i].state);
            }
        }
    };

} // namespace universe::steps

static core::Registrar<universe::ISimulationStep<double, DIMENSION>> reg_rk4(core::string(core::constants::universe::RK4_STEP), [](const core::json& j) {
    return core::make_unique<universe::steps::RK4Step<double, DIMENSION>>();
});

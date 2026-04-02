/**
 * @file PipelineUniverse.h
 * @brief Defines the PipelineUniverse class which implements a pipeline-based simulation universe.
 */
#ifndef PIPELINEUNIVERSE_H
#define PIPELINEUNIVERSE_H

#include "core/Globals.h"
#include "universe/IUniverse.h"
#include "universe/ISimulationStep.h"
#include "core/Registry.h"

/**
 * @namespace universe
 * @brief Simulation universe components and implementations.
 */
namespace universe {

    /**
     * @brief A simulation universe that implements a pipeline-based approach to running simulations.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class PipelineUniverse : public IUniverse<T, D> {
    private:
        core::array<core::unique_ptr<ISimulationStep<T, D>>> m_steps; ///< The sequence of simulation steps to execute.

    public:
        /**
         * @brief Constructs a PipelineUniverse from a JSON configuration.
         * @param j JSON object containing the universe configuration.
         */
        PipelineUniverse(const nlohmann::json& j) {
            if (j.contains(core::constants::universe::PIPELINE_UNIVERSE) && j[core::constants::universe::PIPELINE_UNIVERSE].is_array()) {
                for(const auto& stepName : j[core::constants::universe::PIPELINE_UNIVERSE]) {
                    auto step = core::Registry<ISimulationStep<T, D>>::get().create(stepName, j);
                    if (step) {
                        m_steps.push_back(core::move(step));
                    } else {
                        core::cerr << core::format(core::constants::errors::WARN_STEP_MISSING, stepName.get<core::string>()) << core::nl;
                    }
                }
            }
        }

        /**
         * @brief Executes one simulation step.
         * @param bodies Array of bodies in the universe.
         * @param space The space environment providing physics rules.
         * @param dt Time step size.
         */
        void step(core::array<physics::Body<T, D>>& bodies, const space::ISpace<T, D>& space, T dt) override {
            for(auto& s : m_steps) {
                s -> execute(bodies, space, dt);
            }
        }

        /**
         * @brief Sets the camera target for any simulation steps that need it.
         * @param target Optional name of the body to follow.
         */
        void setCameraTarget(const core::optional<core::string>& target) override {
            for(auto& s : m_steps) {
                s -> setCameraTarget(target);
            }
        }
    };

} // namespace universe

#endif // PIPELINEUNIVERSE_H
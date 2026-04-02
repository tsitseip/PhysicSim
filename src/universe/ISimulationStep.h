/**
 * @file ISimulationStep.h
 * @brief Interface for a single simulation step.
 */

#ifndef ISIMULATIONSTEP_H
#define ISIMULATIONSTEP_H

#include "physics/Physics.h"
#include "space/Space.h"
#include "core/Globals.h"

/**
 * @namespace universe
 * @brief Namespace for universe models and simulation logic.
 */
namespace universe {

    /**
     * @brief Interface for a single simulation step.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, std::size_t D>
    class ISimulationStep {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~ISimulationStep() = default;

        /**
         * @brief Executes the simulation step.
         * @param bodies Collection of bodies to simulate.
         * @param space The spatial model used in the simulation.
         * @param dt The time step size.
         */
        virtual void execute(core::array<physics::Body<T, D>>& bodies,
                             const space::ISpace<T, D>& space,
                             T dt) = 0;

        /**
         * @brief Sets the camera target for the simulation.
         * @param target The identifier of the body to track.
         */
        virtual void setCameraTarget(const core::optional<core::string>& target) {}
    };

} // namespace universe

#endif // ISIMULATIONSTEP_H

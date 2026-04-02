/**
 * @file IUniverse.h
 * @brief Interface for universe models and simulation steps.
 */

#ifndef IUNIVERSE_H
#define IUNIVERSE_H

#include "../core/Globals.h"
#include "physics/Physics.h"
#include "space/Space.h"

/**
 * @namespace universe
 * @brief Namespace for universe models and simulation logic.
 */
namespace universe {

    /**
     * @brief Interface for a universe in the simulation.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, std::size_t D>
    class IUniverse {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IUniverse() = default;

        /**
         * @brief Advances the simulation by one time step.
         * @param bodies The list of physical bodies in the universe.
         * @param space The spatial model to use for calculations.
         * @param dt The time step size.
         */
        virtual void step(core::array<physics::Body<T, D>>& bodies, const space::ISpace<T, D>& space, T dt) = 0;

        /**
         * @brief Sets the camera target for rendering.
         * @param target The name of the body to target, or nullopt for no target.
         */
        virtual void setCameraTarget(const core::optional<core::string>& target) = 0;
    };

} // namespace universe

#endif // IUNIVERSE_H

/**
 * @file StandardUniverse.h
 * @brief Defines the StandardUniverse class which implements a standard simulation universe.
 */
#ifndef STANDARDUNIVERSE_H
#define STANDARDUNIVERSE_H

#include "universe/IUniverse.h"
#include "core/Registry.h"
#include "steps/EulerStep.cpp"
#include "steps/MergingStep.cpp"

/**
 * @namespace universe
 * @brief Simulation universe components and implementations.
 */
namespace universe {

    /**
     * @brief A simulation universe that implements a standard Euler-based approach with body merging.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class StandardUniverse : public IUniverse<T, D> {
        steps::EulerStep<T, D> eulerStep = steps::EulerStep<T, D>(); ///< Fixed Euler step implementation.
        steps::MergingStep<T, D> mergingStep = steps::MergingStep<T, D>(); ///< Fixed Merging step implementation.
    public:
        /**
         * @brief Default constructor for StandardUniverse.
         */
        StandardUniverse() = default;

        /**
         * @brief Executes one simulation step using Euler integration and body merging.
         * @param bodies Array of bodies in the universe.
         * @param space The space environment.
         * @param dt Time step size.
         */
        void step(core::array<physics::Body<T, D>>& bodies, const space::ISpace<T, D>& space, T dt) override {
            eulerStep.execute(bodies, space, dt);
            mergingStep.execute(bodies, space, dt);
        }

        /**
         * @brief StandardUniverse does not use a camera target.
         * @param target Ignored.
         */
        void setCameraTarget(const core::optional<core::string>& target) override {}
    };

} // namespace universe

#endif

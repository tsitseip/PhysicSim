#include "universe/ISimulationStep.h"
#include "core/Registry.h"
#include "core/Globals.h"

namespace universe::steps {

    /**
     * @brief Simulation step for recording and updating body trails.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, core::size_type D>
    class TrailStep : public ISimulationStep<T, D> {
    private:
        int m_counter = 0; ///< Internal counter to track when to record a position.
        int m_frequency = 1; ///< How often to record a position (every N steps).
        core::optional<core::string> m_cameraTarget; ///< Current camera target for relative trail recording.
        core::optional<core::string> m_lastCameraTarget; ///< Stores the previous camera target.
        bool m_targetChanged = false; ///< Flag indicating if the camera target has changed.

    public:
        /**
         * @brief Constructs a TrailStep.
         * @param freq The frequency of trail recording.
         */
        TrailStep(int freq = 1) : m_frequency(freq) {}

        /**
         * @brief Records the positions of active bodies for their trails.
         * @param bodies Array of bodies in the simulation.
         * @param space The space environment.
         * @param dt The time step size.
         */
        void execute(core::array<physics::Body<T, D>>& bodies,
                             const space::ISpace<T, D>& space,
                             T dt) override {
            if (m_targetChanged) {
                for (auto& body : bodies) {
                    body.trail->clear();
                }
                m_targetChanged = false;
            }

            m_counter++;
            if (m_counter < m_frequency) return;
            m_counter = 0;

            math::Vector<T, D> focalPos{};
            bool focalFound = false;

            if (m_cameraTarget) {
                for (const auto& b : bodies) {
                    if (b.name == *m_cameraTarget) {
                        focalPos = b.state.position;
                        focalFound = true;
                        break;
                    }
                }
            }

            for (auto& body : bodies) {
                if (body.active && body.trailEnabled) {
                    math::Vector<T, D> recordPos = body.state.position;
                    
                    if (focalFound) {
                        recordPos = recordPos - focalPos;
                    }
                    body.trail->push_back(recordPos);

                    if (body.trail->size() > body.maxTrailSize) {
                        body.trail->pop_front();
                    }
                }
            }
        }

        /**
         * @brief Sets the camera target for relative trail recording.
         * @param target Optional name of the body to follow.
         */
        void setCameraTarget(const core::optional<core::string>& target) override {
            if (m_cameraTarget != target) {
                m_cameraTarget = target;
                m_targetChanged = true;
            }
        }
    };

} // namespace universe::steps

static core::Registrar<universe::ISimulationStep<double, DIMENSION>> reg_trail(core::string(core::constants::universe::TRAIL_STEP), [](const core::json& j) {
    int freq = j.value(core::constants::universe::trail::TRAIL_FREQ, core::constants::universe::trail::DEFAILT_TRAIL_FREQ);
    return core::make_unique<universe::steps::TrailStep<double, DIMENSION>>(freq);
});

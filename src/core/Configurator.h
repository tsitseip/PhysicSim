/**
 * @file Configurator.h
 * @brief Handles simulation configuration from JSON files.
 */

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include "Globals.h"
#include "Registry.h"

#include "graphics/IRenderer.h"
#include "universe/IUniverse.h"
#include "graphics/GraphicsModule.h"

/**
 * @namespace core
 * @brief Core utilities and classes.
 */
namespace core {

    /**
     * @brief Configurator class to load and provide simulation parameters.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, size_type D>
    class Configurator {
    private:
        json m_data;

    public:
        /**
         * @brief Construct a new Configurator from a JSON file.
         * @param filePath Path to the JSON configuration file.
         * @throws std::runtime_error If the file cannot be opened.
         */
        Configurator(const core::string& filePath) {
            core::ifstream file(filePath);
            if (!file.is_open()) {
                throw std::runtime_error(core::format(core::constants::errors::CONFIG_OPEN_FAIL, filePath));
            }
            file >> m_data;
        }

        /**
         * @brief Construct a new Configurator from an existing JSON object.
         * @param j The JSON object.
         */
        Configurator(const json& j) : m_data(j) {}

        /**
         * @brief Gets the window width from configuration.
         * @return The window width.
         */
        int getWindowWidth() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::WINDOW)) return core::constants::graphics::DEFAULT_WIDTH;
            return m_data[keys::WINDOW].value(keys::WIDTH, core::constants::graphics::DEFAULT_WIDTH);
        }

        /**
         * @brief Gets the window height from configuration.
         * @return The window height.
         */
        int getWindowHeight() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::WINDOW)) return core::constants::graphics::DEFAULT_HEIGHT;
            return m_data[keys::WINDOW].value(keys::HEIGHT, core::constants::graphics::DEFAULT_HEIGHT);
        }

        /**
         * @brief Creates a space object based on configuration.
         * @return Unique pointer to the created space.
         */
        core::unique_ptr<space::ISpace<T, D>> createSpace() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::SPACE)) return nullptr;
            auto s_json = m_data[keys::SPACE];
            core::string type = s_json.value(core::string(keys::TYPE), core::string(keys::DEFAULT_SPACE));
            return Registry<space::ISpace<T, D>>::get().create(type, s_json);
        }

        /**
         * @brief Creates a renderer object based on configuration.
         * @return Unique pointer to the created renderer.
         */
        core::unique_ptr<graphics::IRenderer<T, D>> createRenderer() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::GRAPHICS)) return nullptr;
            auto r_json = m_data[keys::GRAPHICS];
            core::string type = r_json.value(core::string(keys::TYPE), core::string(keys::DEFAULT_GRAPHICS));
            return Registry<graphics::IRenderer<T, D>>::get().create(type, r_json);
        }

        /**
         * @brief Creates a universe object based on configuration.
         * @return Unique pointer to the created universe.
         */
        core::unique_ptr<universe::IUniverse<T, D>> createUniverse() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::UNIVERSE)) return nullptr;
            auto u_json = m_data[keys::UNIVERSE];
            core::string type = u_json.value(core::string(keys::TYPE), core::string(keys::DEFAULT_UNIVERSE));
            return Registry<universe::IUniverse<T, D>>::get().create(type, u_json);
        }

        /**
         * @brief Creates a graphics module object based on configuration.
         * @return Unique pointer to the created graphics module.
         */
        core::unique_ptr<graphics::GraphicsModule<T, D>> createGraphicsModule() const {
            namespace keys = core::constants::json_keys;
            int width = getWindowWidth();
            int height = getWindowHeight();

            auto window = core::make_unique<graphics::Window>(width, height, core::string(core::constants::graphics::WINDOW_TITLE_DEFAULT));
            auto renderer = createRenderer();

            core::optional<core::string> target = nullopt;
            if(m_data.contains(keys::GRAPHICS) && m_data[keys::GRAPHICS].contains(keys::CAMERA_TARGET)) {
                target = m_data[keys::GRAPHICS][keys::CAMERA_TARGET].get<core::string>();
            }

            if (!window || !renderer) return nullptr;
            return core::make_unique<graphics::GraphicsModule<T, D>>(core::move(window), core::move(renderer), target);
        }

        /**
         * @brief Loads physical bodies from configuration.
         * @return Array of bodies.
         */
        core::array<physics::Body<T, D>> loadBodies() const {
            namespace keys = core::constants::json_keys;
            core::array<physics::Body<T, D>> bodies;
            if (!m_data.contains(keys::BODIES)) return bodies;
            int id = 0;


            for (const auto& b_json : m_data[keys::BODIES]) {
                physics::Body<T, D> b;
                auto load_vec = [](const json& j) {
                    core::fixed_array<T, D> arr;
                    for (size_type i = 0; i < D; ++i) arr[i] = j[i];
                    return math::Vector<T, D>(arr);
                };
                b.name = b_json.value(keys::NAME, core::format(keys::DEFAULT_NAME, id));
                b.state.position = load_vec(b_json[keys::POS]);
                b.state.velocity = load_vec(b_json[keys::VEL]);
                b.mass = b_json.value(keys::MASS, core::constants::physics::MASS_DEFAULT);
                b.radius = b_json.value(keys::RADIUS, core::constants::physics::RADIUS_DEFAULT);
                b.active = b_json.value(keys::ACTIVE, true);
                
                b.trailEnabled = b_json.value(keys::TRAIL_ENABLED, false);
                b.maxTrailSize = b_json.value(keys::MAX_TRAIL_SIZE, core::constants::simulation::MAX_TRAIL_SIZE_DEFAULT);

                b.color[0] = b_json.value(keys::RED, keys::DEFAULT_RED);
                b.color[1] = b_json.value(keys::GREEN, keys::DEFAULT_GREEN);
                b.color[2] = b_json.value(keys::BLUE, keys::DEFAULT_BLUE);

                bodies.push_back(b);

                id += 1;
            }
            return bodies;
        }

        /**
         * @brief Gets the simulation time step (delta time).
         * @return The delta time.
         */
        T getDeltaTime() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::UNIVERSE)) return (T)core::constants::simulation::DT_DEFAULT;
            return m_data[keys::UNIVERSE].value(keys::DT, (T)core::constants::simulation::DT_DEFAULT);
        }

        /**
         * @brief Gets the batch size for parallel processing.
         * @return The batch size.
         */
        int getBatchSize() const {
            namespace keys = core::constants::json_keys;
            return m_data.value(keys::BATCH_SIZE, core::constants::simulation::BATCH_SIZE_DEFAULT);
        }

        /**
         * @brief Gets the gravitational constant G.
         * @return The gravitational constant.
         */
        T getG() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::SPACE)) return (T)core::constants::physics::G_DEFAULT;
            return m_data[keys::SPACE].value(keys::G, (T)core::constants::physics::G_DEFAULT);
        }

        /**
         * @brief Gets the speed of light constant C.
         * @return The speed of light.
         */
        T getC() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::SPACE)) return (T)core::constants::physics::C_DEFAULT;
            return m_data[keys::SPACE].value(keys::C, (T)core::constants::physics::C_DEFAULT);
        }

        /**
         * @brief Gets the softening parameter for gravitational calculations.
         * @return The softening parameter.
         */
        T getSoftening() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::SPACE)) return (T)core::constants::physics::SOFTENING_DEFAULT;
            return m_data[keys::SPACE].value(keys::SOFTENING, (T)core::constants::physics::SOFTENING_DEFAULT);
        }

        /**
         * @brief Gets the frequency for recording trail points.
         * @return The trail frequency.
         */
        int getTrailFreq() const {
            namespace keys = core::constants::json_keys;
            if (!m_data.contains(keys::UNIVERSE)) return core::constants::simulation::TRAIL_FREQ_DEFAULT;
            return m_data[keys::UNIVERSE].value(keys::TRAIL_FREQ, core::constants::simulation::TRAIL_FREQ_DEFAULT);
        }
    };

} // namespace core

#endif

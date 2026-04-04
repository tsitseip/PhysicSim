/**
 * @file GraphicsModule.h
 * @brief Defines the GraphicsModule class which manages rendering and user input for the simulation.
 */
#ifndef GRAPHICSMODULE_H
#define GRAPHICSMODULE_H

#include "graphics/Window.h"
#include "graphics/IRenderer.h"
#include "physics/Physics.h"
#include "core/Globals.h"

/**
 * @namespace graphics
 * @brief Graphics rendering components and utilities.
 */
namespace graphics {

    /**
     * @brief GraphicsModule class responsible for managing the rendering process, user input, and camera control in the simulation.
     * @tparam T The numeric type used for calculations (e.g., float, double).
     * @tparam D The dimensionality of the simulation (e.g., 2 for 2D, 3 for 3D).
     */
    template <typename T, std::size_t D>
    class GraphicsModule {
    private:
        core::unique_ptr<Window> m_window; ///< The application window.
        core::unique_ptr<IRenderer<T, D>> m_renderer; ///< The renderer used for drawing bodies.
        core::optional<core::string> m_cameraTarget; ///< The name of the body the camera is following.
        math::Matrix<T, D> m_cameraRotation = math::Matrix<T, D>::identity(); ///< The current camera rotation matrix.

        bool m_showDebug = false; ///< Flag to toggle debug information display.
        bool m_f3PressedLast = false; ///< Tracks the state of the F3 key for toggling debug info.

        core::atomic<bool> m_paused = false; /// < Flag to toggle pause/unpause physics
        bool m_spacePressedLast = false; /// tracks the state of the space key for toggling the paused info.
        double m_zoom = 1; /// Current zoom level for the camera.

        T m_speed = (T)core::constants::graphics::INPUT_SPEED_DEFAULT;

    public:
        /**
         * @brief Constructs a GraphicsModule.
         * @param window Unique pointer to the Window object.
         * @param renderer Unique pointer to the IRenderer object.
         * @param cameraTarget Optional name of the body to target with the camera.
         */
        GraphicsModule( core::unique_ptr<Window> window, 
                        core::unique_ptr<IRenderer<T, D>> renderer,
                        core::optional<core::string> cameraTarget = std::nullopt)
            : m_window(core::move(window)), m_renderer(core::move(renderer)), m_cameraTarget(cameraTarget) {
                auto window_ = m_window->getHandle();
                glfwSetWindowUserPointer(window_, this);
                glfwSetScrollCallback(window_, scroll_callback);
            }

        /**
         * @brief Checks if the window is still open.
         * @return True if the window is open, false otherwise.
         */
        bool isOpen() const { return !m_window->shouldClose(); }

        /**
         * @brief Clears the window for the next frame.
         */
        void clear() { m_window->clear(); }
        
        /**
         * @brief Renders the simulation scene.
         * @param bodies The array of physical bodies to render.
         */
        void render(const core::array<physics::Body<T, D>>& bodies) {
            math::Vector<T, D> posOffset{};
            
            if (m_cameraTarget) {
                for (const auto& body : bodies) {
                    if (body.name == m_cameraTarget.value()) {
                        posOffset = body.state.position;
                        break;
                    }
                }
            }

            m_renderer->begin();
            for (const auto& body : bodies) {
                m_renderer->draw(body, posOffset, m_cameraRotation, m_zoom);
            }
            m_renderer->end();
        }

        /**
         * @brief Updates the window by swapping buffers and polling events.
         */
        void update() {
            m_window->swapBuffers();
            m_window->pollEvents();
        }

        void processScrollEvent(double yoffset) {
            if (yoffset > 0) {
                m_zoom *= 1.1; // Zoom in by increasing the zoom factor
            } else if (yoffset < 0) {
                m_zoom *= 0.9; // Zoom out by decreasing the zoom factor
            }
            // std::cout << "Zoom updated to: " << m_zoom << "\n";
        }

        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
            // if (yoffset > 0) {
            //     std::cout << "Scrolled UP. Offset: " << yoffset << "\n";
            // } else if (yoffset < 0) {
            //     std::cout << "Scrolled DOWN. Offset: " << yoffset << "\n";
            // }

            // if (xoffset != 0) {
            //     std::cout << "Scrolled HORIZONTALLY. Offset: " << xoffset << "\n";
            // }

            GraphicsModule* instance = static_cast<GraphicsModule*>(glfwGetWindowUserPointer(window));

            if (instance) {
                instance->processScrollEvent(yoffset);
            }
        }

        /**
         * @brief Handles user input for camera control and debug display toggling.
         * @param bodies The array of physical bodies in the simulation (used for camera targeting).
         */
        void handleInput(core::array<physics::Body<T, D>>& bodies) {
            GLFWwindow* window = m_window->getHandle();

            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }

            if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
                if (!m_f3PressedLast) {
                    m_showDebug = !m_showDebug;
                    if (!m_showDebug) glfwSetWindowTitle(window, core::string(core::constants::graphics::WINDOW_TITLE_DEFAULT).c_str());
                    m_f3PressedLast = true;
                }
            } else { m_f3PressedLast = false; }

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                if (!m_spacePressedLast) {
                    m_paused = !m_paused;
                    m_spacePressedLast = true;
                }
            } else {
                m_spacePressedLast = false;
            }

            bool shifted = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
            T speed = m_speed;

            core::array<int> held_dims;
            for(int d=0; d<D; ++d) {
                if (glfwGetKey(window, GLFW_KEY_1 + d) == GLFW_PRESS) {
                    held_dims.push_back(d);
                }
            }

            if(held_dims.size() == 2){
                int i = held_dims[0];
                int j = held_dims[1];
                T angle = shifted ? -speed : speed;

                m_cameraRotation *= math::Matrix<T, D>::rotation(i, j, angle);
            }
        }

        /**
         * @brief Updates the HUD with current FPS and TPS.
         * @param fps Current frames per second.
         * @param tps Current ticks per second.
         */
        void updateHUD(int fps, int tps) {
            if (m_showDebug) {
                core::cout << core::format(core::constants::ui::HUD_FORMAT, fps, tps) << core::nl << std::flush;
                glfwSetWindowTitle(m_window->getHandle(), core::format(core::constants::ui::HUD_FORMAT, fps, tps).c_str());
            }
            m_speed = 1.0 / fps;
        }

        /**
         * @brief Returns the underlying GLFW window handle.
         * @return Pointer to the GLFWwindow.
         */
        GLFWwindow* getWindowHandle() const { return m_window->getHandle(); }

        /**
         * @brief Gets the current camera target name.
         * @return Optional string containing the target name.
         */
        core::optional<core::string> getCameraTarget() const { return m_cameraTarget; }

        bool isPaused() const {
            return m_paused;
        }
    };

} // namespace graphics

#endif

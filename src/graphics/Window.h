#ifndef WINDOW_H
#define WINDOW_H

#include "../core/Globals.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * @namespace graphics
 * @brief Graphics rendering components and utilities.
 */
namespace graphics {

    /**
     * @brief A class that encapsulates the GLFW windowing and OpenGL context creation.
     */
    class Window {
    private:
        /**
         * @brief Pointer to the GLFW window handle.
         */
        GLFWwindow* m_window;

        /**
         * @brief Width of the window in pixels.
         */
        int m_width;

        /**
         * @brief Height of the window in pixels.
         */
        int m_height;

        /**
         * @brief Title of the window.
         */
        core::string m_title;

    public:
        /**
         * @brief Constructs a new Window and initializes GLFW and GLAD.
         * 
         * Sets up the OpenGL context, window hints, and basic OpenGL states like depth testing.
         * 
         * @param width The desired width of the window. If <= 0, the primary monitor's width is used.
         * @param height The desired height of the window. If <= 0, the primary monitor's height is used.
         * @param title The title of the window.
         */
        Window(int width, int height, const core::string& title) 
            : m_title(title) {
            
            if (!glfwInit()) {
                core::cerr << core::constants::errors::FAIL_INIT_GLFW << core::nl;
                exit(-1);
            }

            core::cout<< core::format(core::constants::window::INIT, title, width, height) << core::nl;

            GLFWmonitor* primary = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(primary);
            if (width <= 0) {
                m_width = mode->width;
            }
            else {
                m_width = width;
            }

            if (height <= 0) {
                m_height = mode->height;
            }
            else {
                m_height = height;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
            if (!m_window) {
                core::cerr << core::constants::errors::FAIL_INIT_GLFW << core::nl;
                glfwTerminate();
                exit(-1);
            }

            glfwMakeContextCurrent(m_window);
            
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                core::cerr << core::constants::errors::FAIL_INIT_GLAD << core::nl;
                exit(-1);
            }

            glEnable(GL_PROGRAM_POINT_SIZE); // Enable control over point size in vertex shader
            glEnable(GL_DEPTH_TEST); // Enable depth testing for correct 3D occlusion
            glDepthFunc(GL_LESS); // Nearer objects should be drawn over further ones
        }

        /**
         * @brief Cleans up GLFW resources and terminates the windowing system on destruction.
         */
        ~Window() {
            glfwTerminate(); // Clean up GLFW resources on destruction
        }

        /**
         * @brief Checks if the window is scheduled to close.
         * @return True if the window should close, false otherwise.
         */
        bool shouldClose() const {
            return glfwWindowShouldClose(m_window); // Check if the window should close
        }

        /**
         * @brief Clears the color and depth buffers with the specified color.
         * @param r Red component (0.0 - 1.0).
         * @param g Green component (0.0 - 1.0).
         * @param b Blue component (0.0 - 1.0).
         * @param a Alpha component (0.0 - 1.0).
         */
        void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) {
            glClearColor(r, g, b, a); // Set the background color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear both color and depth buffers for the new frame
        }

        /**
         * @brief Swaps the front and back buffers of the window.
         */
        void swapBuffers() {
            glfwSwapBuffers(m_window);
        }

        /**
         * @brief Polls for pending window events (e.g., input, resizing).
         */
        void pollEvents() {
            glfwPollEvents();
        }

        /**
         * @brief Returns the underlying GLFW window handle.
         * @return Pointer to the GLFWwindow.
         */
        GLFWwindow* getHandle() const { return m_window; }
    };

} // namespace graphics

#endif

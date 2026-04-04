/**
 * @file CircleRenderer.h
 * @brief Declaration and implementation of the CircleRenderer class for rendering circular bodies and their trails.
 */
#ifndef CIRCLERENDERER_H
#define CIRCLERENDERER_H

#include <glad/glad.h>
#include "graphics/IRenderer.h"
#include "graphics/Shader.h"
#include "graphics/Render.h"

/**
 * @namespace graphics
 * @brief Graphics rendering components and utilities.
 */
namespace graphics {

    inline const char* CIRCLE_V_SRC = "#version 410 core\n"
                               "layout (location = 0) in vec3 aPos;\n"
                               "layout (location = 1) in float aPointSize;\n"
                               "layout (location = 2) in vec4 aColor;\n"
                               "out vec4 vColor;\n"
                               "void main() { gl_Position = vec4(aPos, 1.0); gl_PointSize = aPointSize; vColor = aColor; }";

    inline const char* CIRCLE_F_SRC = "#version 410 core\n"
                               "out vec4 FragColor;\n"
                               "in vec4 vColor;\n"
                               "void main() {\n"
                               "    if (length(gl_PointCoord - vec2(0.5)) > 0.5) discard;\n"
                               "    FragColor = vColor;\n"
                               "}";

    inline const char* TRAIL_V_SRC = "#version 410 core\n"
                               "layout (location = 0) in vec3 aPos;\n"
                               "layout (location = 1) in vec4 aColor;\n"
                               "out vec4 vColor;\n"
                               "void main() {\n"
                               "    gl_Position = vec4(aPos, 1.0);\n"
                               "    vColor = aColor;\n"
                               "}";

    inline const char* TRAIL_F_SRC = "#version 410 core\n"
                               "in vec4 vColor;\n"
                               "out vec4 FragColor;\n"
                               "void main() { FragColor = vColor; }";
    /**
     * @brief CircleRenderer class responsible for rendering circular bodies and their trails in the simulation.
     * 
     * Uses OpenGL point sprites to render bodies and line strips for trails.
     * 
     * @tparam T The numeric type used for calculations (e.g., float, double).
     * @tparam D The dimensionality of the simulation (e.g., 2 for 2D, 3 for 3D).
     */
    template <typename T, std::size_t D>
    class CircleRenderer : public IRenderer<T, D> {
    private:
        /**
         * @brief Vertex Array Object for rendering bodies.
         */
        unsigned int VAO_body = 0;

        /**
         * @brief Vertex Buffer Object for rendering bodies.
         */
        unsigned int VBO_body = 0;

        /**
         * @brief Vertex Array Object for rendering trails.
         */
        unsigned int VAO_trail = 0;

        /**
         * @brief Vertex Buffer Object for rendering trails.
         */
        unsigned int VBO_trail = 0;

        /**
         * @brief Shader program for rendering circular bodies.
         */
        Shader m_shader;

        /**
         * @brief Shader program for rendering trails.
         */
        Shader m_trailShader;

        /**
         * @brief Focal length used for 3D to 2D projection.
         */
        T m_focalLength;

    public:
        /**
         * @brief Constructs a new CircleRenderer and initializes OpenGL buffers and shaders.
         * @param focalLength The focal length for projection.
         */
        CircleRenderer(T focalLength = core::constants::graphics::FOCAL_LENGTH_DEFAULT) 
            : m_shader(CIRCLE_V_SRC, CIRCLE_F_SRC), m_trailShader(TRAIL_V_SRC, TRAIL_F_SRC), m_focalLength(focalLength) {
            glGenVertexArrays(1, &VAO_body);
            glGenBuffers(1, &VBO_body);
            glGenVertexArrays(1, &VAO_trail);
            glGenBuffers(1, &VBO_trail);
        }

        /**
         * @brief Deletes OpenGL buffers on destruction.
         */
        ~CircleRenderer() {
            glDeleteVertexArrays(1, &VAO_body);
            glDeleteBuffers(1, &VBO_body);
            glDeleteVertexArrays(1, &VAO_trail);
            glDeleteBuffers(1, &VBO_trail);
        }

        /**
         * @brief Prepares the renderer for drawing by activating the body shader.
         */
        void begin() override { m_shader.use(); }

        /**
         * @brief Finalizes drawing (currently no operations needed).
         */
        void end() override {}

        /**
         * @brief Draws a single physical body as a circle and its trail if enabled.
         * @param body The body to draw.
         * @param offset The camera offset vector.
         * @param rotation The camera rotation matrix.
         */
        void draw(const physics::Body<T, D>& body, 
                  const math::Vector<T, D>& offset, 
                  const math::Matrix<T, D>& rotation, 
                  double zoom) override {
            
            if (body.trailEnabled && !body.trail.empty() && body.active) {
                m_trailShader.use();
                core::array<float> trailBuffer;
                trailBuffer.reserve(body.trail.size() * 7);
                float id = 0;
                float total = (float)body.trail.size();
                for (const auto& trailPos : body.trail) {
                    math::Vector<T, D> pos = trailPos * rotation * zoom;
                    math::Vector<T, 3> p = projectTo3D(pos, m_focalLength);
                    
                    trailBuffer.push_back((float)p[0]); 
                    trailBuffer.push_back((float)p[1]);  
                    trailBuffer.push_back((float)p[2]); 
                    trailBuffer.push_back((float)body.color[0]); 
                    trailBuffer.push_back((float)body.color[1]); 
                    trailBuffer.push_back((float)body.color[2]); 
                    trailBuffer.push_back(1.0f); 
                }

                glBindVertexArray(VAO_trail); 
                glBindBuffer(GL_ARRAY_BUFFER, VBO_trail); 
                glBufferData(GL_ARRAY_BUFFER, trailBuffer.size() * sizeof(float), trailBuffer.data(), GL_DYNAMIC_DRAW);
                
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0); 
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);

                glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)body.trail.size());
                m_shader.use();
            }

            if (!body.active) return;

            math::Vector<T, D> relativePos = (body.state.position - offset) * rotation * zoom;
            math::Vector<T, 3> projected = projectTo3D(relativePos, m_focalLength); 

            T w = (D >= 3) ? (relativePos[D-1] + m_focalLength) : m_focalLength;
            T projectedRadius = body.radius * (m_focalLength / w) * zoom;
            float pixelSize = (float)(projectedRadius * (float)core::constants::graphics::PIXEL_SCALING_FACTOR); 
            
            float buffer[8] = { 
                (float)projected[0], (float)projected[1], (float)projected[2],
                pixelSize,
                (float)body.color[0], (float)body.color[1], (float)body.color[2], 1.0f
            };
            
            glBindVertexArray(VAO_body);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_body);
            glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0); 
            glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1); 
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glDrawArrays(GL_POINTS, 0, 1);
        }
    };

} // namespace graphics

#endif

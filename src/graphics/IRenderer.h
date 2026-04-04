/**
 * @file IRenderer.h
 * @brief Interface for rendering bodies in the simulation.
 */

#ifndef IRENDERER_H
#define IRENDERER_H

#include "physics/Physics.h"

/**
 * @namespace graphics
 * @brief Namespace for rendering and graphical operations.
 */
namespace graphics {

    /**
     * @brief Interface for a renderer that draws bodies.
     * @tparam T The numeric type.
     * @tparam D The dimensionality.
     */
    template <typename T, std::size_t D>
    class IRenderer {
    public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IRenderer() = default;
        
        /**
         * @brief Draws a single physical body.
         * @param body The body to draw.
         * @param offset The camera offset vector.
         * @param rotation The camera rotation matrix.
         */
        virtual void draw(const physics::Body<T, D>& body, 
                          const math::Vector<T, D>& offset = math::Vector<T, D>{}, 
                          const math::Matrix<T, D>& rotation = math::Matrix<T, D>::identity(),
                          double zoom = 1.0) = 0;
        
        /**
         * @brief Prepares the renderer for drawing.
         */
        virtual void begin() = 0;

        /**
         * @brief Finalizes the rendering process.
         */
        virtual void end() = 0;
    };

} // namespace graphics

#endif // IRENDERER_H

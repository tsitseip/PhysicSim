/**
 * @file Render.h
 * @brief Provides utility functions for rendering, including projection of higher-dimensional points to 3D space.
 */
#ifndef RENDER_H
#define RENDER_H

#include "math/Math.h"

/**
 * @namespace graphics
 * @brief Graphics rendering components and utilities.
 */
namespace graphics {
    /**
     * @brief Projects a point from D-dimensional space to 3D space using perspective projection.
     * @tparam T The numeric type used for calculations (e.g., float, double).
     * @tparam D The dimensionality of the input point (e.g., 2 for 2D, 4 for 4D).
     * @param pos The position vector in D-dimensional space.
     * @param focalLength The focal length used for perspective projection (applicable when D > 3).
     * @return The projected position in 3D space.
     */
    template<typename T, std::size_t D>
    math::Vector<T, 3> projectTo3D(const math::Vector<T, D>& pos, T focalLength) {
        if constexpr (D == 3) {
            return pos;
        } else if constexpr (D > 3) {
            T w = pos[D-1] + focalLength;
            math::Vector<T, D-1> lower;
            for(size_t i = 0; i < D-1; ++i) {
                lower[i] = pos[i] * (focalLength / w);
            }
            return projectTo3D(lower, focalLength);
        } else if constexpr (D==2){
            return math::Vector<T, 3>({pos[0], pos[1], T(0)});
        } else if constexpr (D==1){
            return math::Vector<T, 3>({pos[0], T(0), T(0)});
        } else {
            return math::Vector<T, 3>({T(0), T(0), T(0)});
        }
    }
} // namespace graphics

#endif // RENDER_H
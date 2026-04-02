/**
 * @file CircleRenderer.cpp
 * @brief Registration of the CircleRenderer class for rendering circular bodies and their trails.
 */
#include "CircleRenderer.h"
#include "core/Registry.h"
#include "core/Globals.h"


static core::Registrar<graphics::IRenderer<double, DIMENSION>> reg(core::string(core::constants::json_keys::DEFAULT_GRAPHICS), [](const core::json& j) {
    return core::make_unique<graphics::CircleRenderer<double, DIMENSION>>(
        j.value(core::constants::graphics::FOCAL_LENGTH, core::constants::graphics::DEFAULT_FOCAL_LENGTH)
    );
});
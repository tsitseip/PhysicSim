#include "universe/PipelineUniverse.h"
#include "core/Registry.h"
#include "core/Globals.h"

static core::Registrar<universe::IUniverse<double, DIMENSION>> reg(core::string(core::constants::universe::PIPELINE_UNIVERSE), [](const core::json& j) {
    return core::make_unique<universe::PipelineUniverse<double, DIMENSION>>(j);
});

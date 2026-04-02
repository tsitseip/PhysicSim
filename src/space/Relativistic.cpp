#include "Relativistic.h"
#include "core/Registry.h"


static core::Registrar<space::ISpace<double, DIMENSION>> reg(core::string(core::constants::space::SPACE_RELATIVISTIC), [](const core::json& j) {
    return core::make_unique<space::relativistic::EinsteinSpace<double, DIMENSION>>(
        j.value(core::constants::space::relativistic::G,         core::constants::space::relativistic::G_DEFAULT), 
        j.value(core::constants::space::relativistic::C,         core::constants::space::relativistic::C_DEFAULT), 
        j.value(core::constants::space::relativistic::SOFTENING, core::constants::space::relativistic::SOFTENING_DEFAULT)
    );
});
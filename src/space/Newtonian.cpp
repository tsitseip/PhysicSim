#include "Newtonian.h"
#include "core/Registry.h"
#include "core/Globals.h"


static core::Registrar<space::ISpace<double, DIMENSION>> reg(core::string(core::constants::space::SPACE_NEWTONIAN), [](const core::json& j) {
    return core::make_unique<space::newtonian::GravitySpace<double, DIMENSION>>(
        j.value(core::constants::space::newtonian::G,            core::constants::space::newtonian::G_DEFAULT), 
        j.value(core::constants::space::newtonian::SOFTENING,    core::constants::space::newtonian::SOFTENING_DEFAULT)
    );
});
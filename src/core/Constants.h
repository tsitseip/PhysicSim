#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string_view>

namespace core::constants {
    constexpr char nl = '\n';
    
    namespace physics {
        constexpr double G_DEFAULT = 1.0;
        constexpr double C_DEFAULT = 100.0;
        constexpr double SOFTENING_DEFAULT = 0.01;
        constexpr double MASS_DEFAULT = 1.0;
        constexpr double RADIUS_DEFAULT = 0.05;
    } // namespace physics

    namespace simulation {
        constexpr double DT_DEFAULT = 0.001;
        constexpr int BATCH_SIZE_DEFAULT = 100;
        constexpr double MERGE_GROWTH_FACTOR = 0.1;
        constexpr int TRAIL_FREQ_DEFAULT = 1;
        constexpr int MAX_TRAIL_SIZE_DEFAULT = 100;
    } // namespace simulation

    namespace space {
        constexpr std::string_view SPACE_NEWTONIAN = "newtonian";
        constexpr std::string_view SPACE_RELATIVISTIC = "relativistic";

        namespace newtonian {
            constexpr std::string_view G = "G";
            constexpr std::string_view SOFTENING = "softening";
            constexpr double G_DEFAULT = 1.0;
            constexpr double SOFTENING_DEFAULT = 0.01;
        } // namespace newtonian

        namespace relativistic {
            constexpr std::string_view G = "G";
            constexpr std::string_view SOFTENING = "softening";
            constexpr std::string_view C = "c";
            constexpr double C_DEFAULT = 100.0;
            constexpr double G_DEFAULT = 1.0;
            constexpr double SOFTENING_DEFAULT = 0.01;

            constexpr double EIH_ALPHA = 4.0;
            constexpr double EIH_BETA = 1.5;
            constexpr double EIH_GAMMA = 3.0;
            constexpr double EIH_DELTA = 1.0;
        } // namespace relativistic
    } // namespace space

    namespace graphics {
        constexpr std::string_view GRAPHICS_CIRCLE = "circle";
        constexpr double PIXEL_SCALING_FACTOR = 1000.0;
        constexpr double FOCAL_LENGTH_DEFAULT = 5.0;
        constexpr int DEFAULT_WIDTH = 0;
        constexpr int DEFAULT_HEIGHT = 0;
        constexpr std::string_view WINDOW_TITLE_DEFAULT = "N-Dimensional Simulator";
        constexpr double INPUT_SPEED_DEFAULT = 0.01;
        constexpr std::string_view FOCAL_LENGTH = "focalLength";
        constexpr double DEFAULT_FOCAL_LENGTH = 5.0;
    } // namespace graphics

    namespace window {
        constexpr std::string_view INIT = "Creating window: {} ({}x{})";
    } // namespace window

    namespace universe {
        constexpr std::string_view PIPELINE_UNIVERSE = "pipeline";
        constexpr std::string_view STANDART_UNIVERSE = "standard";
        constexpr std::string_view EULER_STEP = "euler";
        constexpr std::string_view RK4_STEP = "rk4";
        constexpr std::string_view MERGING_STEP = "merging";
        constexpr std::string_view TRAIL_STEP = "trail";

        namespace trail {
            constexpr std::string_view TRAIL_FREQ = "trailFreq";
            constexpr int DEFAILT_TRAIL_FREQ = 100;
        } // namespace trail
        
    } // namespace universe

    namespace time {
        constexpr double MS_PER_SEC = 1000.0;
    } // namespace time

    namespace json_keys {
        constexpr std::string_view WINDOW                  = "window";
        constexpr std::string_view WIDTH                   = "width";
        constexpr std::string_view HEIGHT                  = "height";
        constexpr std::string_view TITLE                   = "title";
        constexpr std::string_view UNIVERSE                = "universe";
        constexpr std::string_view DEFAULT_UNIVERSE        = universe::STANDART_UNIVERSE;
        constexpr std::string_view TYPE                    = "type";
        constexpr std::string_view SPACE                   = "space";
        constexpr std::string_view DEFAULT_SPACE           = space::SPACE_NEWTONIAN;
        constexpr std::string_view GRAPHICS                = "graphics";
        constexpr std::string_view DEFAULT_GRAPHICS        = graphics::GRAPHICS_CIRCLE;
        constexpr std::string_view CAMERA_TARGET           = "cameraTarget";
        constexpr std::string_view BODIES                  = "bodies";
        constexpr std::string_view NAME                    = "name";
        constexpr std::string_view DEFAULT_NAME            = "objectN{}";
        constexpr std::string_view POS                     = "pos";
        constexpr std::string_view VEL                     = "vel";
        constexpr std::string_view MASS                    = "mass";
        constexpr std::string_view RADIUS                  = "radius";
        constexpr std::string_view ACTIVE                  = "active";
        constexpr std::string_view TRAIL_ENABLED           = "trail_enabled";
        constexpr std::string_view MAX_TRAIL_SIZE          = "max_trail_size";
        constexpr std::string_view RED                     = "red";
        constexpr std::string_view GREEN                   = "green";
        constexpr std::string_view BLUE                    = "blue";
        constexpr double DEFAULT_RED                       = 0.0;
        constexpr double DEFAULT_GREEN                     = 0.0;
        constexpr double DEFAULT_BLUE                      = 0.0;
        constexpr std::string_view DT                      = "dt";
        constexpr std::string_view BATCH_SIZE              = "batchSize";
        constexpr std::string_view G                       = "G";
        constexpr std::string_view C                       = "c";
        constexpr std::string_view SOFTENING               = "softening";
        constexpr std::string_view TRAIL_FREQ              = "trailFreq";
    } // namespace json_keys

    namespace errors {
        constexpr std::string_view CONFIG_OPEN_FAIL  = "Could not open config file: {}";
        constexpr std::string_view MODULE_INIT_FAIL  = "CRITICAL ERROR: Failed to initialize modules.";
        constexpr std::string_view GENERIC_CRITICAL  = "CRITICAL ERROR: {}";
        constexpr std::string_view REGISTRY_ERROR    = "Registry Error: No creator registered for '{}'";
        constexpr std::string_view WARN_STEP_MISSING = "Warning: Simulation Step '{}' not found.";
        constexpr std::string_view FAIL_INIT_GLFW    = "Failed to initialize GLFW";
        constexpr std::string_view FAIL_INIT_GLAD    = "Failed to initialize GLAD";
    } // namespace errors

    namespace ui {
        constexpr std::string_view START_MSG = "Starting Simulation in {}D";
        constexpr std::string_view HUD_FORMAT = "FPS: {} | TPS: {}";
    } // namespace ui

} // namespace core::constants

#endif // CONSTANTS_H

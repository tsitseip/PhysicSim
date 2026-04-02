cmake_minimum_required(VERSION 3.20)
project(PhysicsSim LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Dimension Setup
set(DIMENSION 3 CACHE STRING "Dimension of the simulation space")
add_definitions(-DDIMENSION=${DIMENSION})
message(STATUS "Building for Dimension: ${DIMENSION}")

# 2. Find Dependencies
find_package(OpenGL REQUIRED)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(json)
if(NOT TARGET nlohmann_json::nlohmann_json)
    add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
endif()

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# 3. Sources
include_directories(. include)
file(GLOB_RECURSE MODULE_SOURCES "space/*.cpp" "graphics/*.cpp" "universe/*.cpp")

add_executable(PhysicsSim 
    main.cpp 
    glad.c 
    ${MODULE_SOURCES}
)

# 4. Link Libraries
target_link_libraries(PhysicsSim 
    PRIVATE
    glfw
    OpenGL::GL
    nlohmann_json::nlohmann_json
)

# 5. Windows vs Linux Specifics
if(WIN32)
    # Copy config.json to the same folder as the .exe so it's always found
    add_custom_command(TARGET PhysicsSim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config.json"
        "$<TARGET_FILE_DIR:PhysicsSim>/config.json")
else()
    # On Linux, ensure we link against system libraries if needed
    target_link_libraries(PhysicsSim PRIVATE pthread dl)
endif()

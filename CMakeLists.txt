cmake_minimum_required(VERSION 3.20)
project(vscode_glfw_basic
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- OpenGL ---
find_package(OpenGL REQUIRED)

# --- GLFW ---
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9   # Latest 3.3.x release
)

# --- GLFW - Disable unnecessary bloat ---
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

add_executable(vscode_glfw_basic src/main.cpp)

target_link_libraries(vscode_glfw_basic
    PRIVATE
        OpenGL::GL
        glfw
)
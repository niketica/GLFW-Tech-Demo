cmake_minimum_required(VERSION 3.20)
project(vscode_glfw_basic
    VERSION 0.1.0
    LANGUAGES C CXX
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

# --- ZLIB ---
find_package(ZLIB QUIET)

if (NOT ZLIB_FOUND)
    message(STATUS "ZLIB not found, fetching with FetchContent")

    include(FetchContent)
    
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    
    FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/madler/zlib.git
        GIT_TAG v1.3.1
    )

    set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_TESTS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(zlib)

    # Create an alias in order to use ZLIB::ZLIB
    add_library(ZLIB::ZLIB ALIAS zlib)
endif()

add_executable(vscode_glfw_basic src/main.cpp)

target_include_directories(vscode_glfw_basic PRIVATE
    vendor/glad/include
    vendor/entt/include
    vendor/glm/include
    src
)

add_library(engine
    # glad
    vendor/glad/src/glad.c
    
    # src/engine
    src/engine/Engine.cpp
    
    # src/asset
    src/asset/FileLoader.cpp
    src/asset/IAssetLoader.cpp
    src/asset/PakReader.cpp
    src/asset/EncryptionUtil.cpp

    # src/renderer
    src/renderer/Shader.cpp
    src/renderer/SimpleRenderer.cpp
    src/renderer/RendererRepository.cpp

    # src/systems
    src/systems/SystemRepository.cpp
)

target_include_directories(engine PUBLIC
    vendor/glad/include
    vendor/entt/include
    vendor/glm/include
    src
)

target_link_libraries(engine
    PUBLIC
        OpenGL::GL
        glfw
        ZLIB::ZLIB
)

target_link_libraries(vscode_glfw_basic
    PRIVATE
        OpenGL::GL
        glfw
        ZLIB::ZLIB
        engine
)

add_executable(compressor src/compressor/main.cpp)

target_link_libraries(compressor
    PRIVATE
        ZLIB::ZLIB
)
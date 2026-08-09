cmake_minimum_required(VERSION 3.20)
# Fix for CMake 4.x + older subprojects (freetype, zlib, etc.)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(vscode_glfw_basic
    VERSION 0.1.0
    LANGUAGES C CXX
)

cmake_policy(SET CMP0072 NEW)
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

# --- Freetype ---
find_package(Freetype QUIET)

if (NOT Freetype_FOUND)
    message(STATUS "Freetype not found, fetching with FetchContent")

    include(FetchContent)
    
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-13-2
    )

    # Tell freetype to use our zlib
    set(FT_DISABLE_BZIP2 TRUE CACHE BOOL "" FORCE)
    set(FT_DISABLE_PNG TRUE CACHE BOOL "" FORCE)
    set(FT_DISABLE_HARFBUZZ TRUE CACHE BOOL "" FORCE)
    set(FT_DISABLE_BROTLI TRUE CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(freetype)

    # Normalize the target name
    add_library(Freetype::Freetype ALIAS freetype)
endif()

add_executable(vscode_glfw_basic src/main.cpp)

target_include_directories(vscode_glfw_basic PRIVATE
    vendor/glad/include
    vendor/entt/include
    vendor/glm/include
    vendor/stb/include
    vendor/miniaudio/include
    src
)

add_library(engine
    # glad
    vendor/glad/src/glad.c
    
    # src/engine
    src/engine/core/Engine.cpp
    src/engine/builder/userinterface/UITextLabelBuilder.cpp
    src/engine/builder/userinterface/UIButtonBuilder.cpp
    src/engine/builder/userinterface/UIPanelBuilder.cpp
    src/engine/adapter/asset/FileLoader.cpp
    src/engine/adapter/asset/IAssetLoader.cpp
    src/engine/adapter/asset/PakReader.cpp
    src/engine/adapter/asset/EncryptionUtil.cpp
    src/engine/adapter/renderer/Shader.cpp
    src/engine/adapter/renderer/TextureLoader.cpp
    src/engine/adapter/renderer/SpriteInstancedRenderer.cpp
    src/engine/adapter/renderer/NineSliceInstancedRenderer.cpp
    src/engine/adapter/renderer/RenderContext.cpp
    src/engine/adapter/renderer/text/Font.cpp
    src/engine/adapter/renderer/text/TextRenderer.cpp
    src/engine/adapter/renderer/RectangleRenderer.cpp
    src/engine/adapter/renderer/RectangleBorderRenderer.cpp
    src/engine/adapter/sound/SoundBackendMiniaudio.cpp
    src/engine/adapter/systems/common/WorldTransformSystem.cpp
    src/engine/adapter/systems/input/InputSystem.cpp
    src/engine/adapter/systems/sound/SoundSystem.cpp
    src/engine/adapter/systems/renderer/SpriteRendererSystem.cpp
    src/engine/adapter/systems/renderer/TextRendererSystem.cpp
    src/engine/adapter/systems/renderer/ResolutionSystem.cpp
    src/engine/adapter/systems/userinterface/UILayoutSystem.cpp
    src/engine/adapter/systems/userinterface/UINavigationSystem.cpp
    src/engine/adapter/systems/SystemContext.cpp
    src/engine/adapter/scene/SceneContext.cpp
    src/engine/adapter/scene/MainMenuScene.cpp
    src/engine/adapter/scene/SnakeScene.cpp
)

target_include_directories(engine PUBLIC
    vendor/glad/include
    vendor/entt/include
    vendor/glm/include
    vendor/stb/include
    vendor/miniaudio/include
    src
)

target_link_libraries(engine
    PUBLIC
        OpenGL::GL
        glfw
        ZLIB::ZLIB
        Freetype::Freetype
)

target_link_libraries(vscode_glfw_basic
    PRIVATE
        OpenGL::GL
        glfw
        ZLIB::ZLIB
        Freetype::Freetype
        engine
)

add_executable(compressor src/compressor/main.cpp)

target_link_libraries(compressor
    PRIVATE
        ZLIB::ZLIB
)
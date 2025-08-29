## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
##
## This work is licensed under the terms of the MIT license.
##
## For a copy, see <https://opensource.org/licenses/MIT>.
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

CMAKE_MINIMUM_REQUIRED(VERSION 3.26)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Project
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

PROJECT(Aurora_Engine VERSION 1.0.0 LANGUAGES CXX DESCRIPTION "Aurora Engine Module")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Option
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

OPTION(AURORA_PROFILE_MODE "Enable Profile Mode" OFF)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Code
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FILE(GLOB_RECURSE PROJECT_SOURCE "Public/*.cpp" "Private/*.cpp")

IF    (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    LIST(FILTER PROJECT_SOURCE EXCLUDE REGEX ".*D3D11.*")
ENDIF (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Includes
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LIST(APPEND PROJECT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Public ${CMAKE_CURRENT_SOURCE_DIR}/Private)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Configuration
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LIST(APPEND PROJECT_DEFINES "AURORA_LOADER_MP3")
LIST(APPEND PROJECT_DEFINES "AURORA_LOADER_WAV")
LIST(APPEND PROJECT_DEFINES "AURORA_LOADER_STB")
LIST(APPEND PROJECT_DEFINES "AURORA_LOADER_MATERIAL")
LIST(APPEND PROJECT_DEFINES "AURORA_LOADER_GLTF")
LIST(APPEND PROJECT_DEFINES "AURORA_LOADER_EFFECT")
LIST(APPEND PROJECT_DEFINES "AURORA_LOADER_ARTERY")

IF    (AURORA_PROFILE_MODE)
    LIST(APPEND PROJECT_DEFINES "AURORA_PROFILE_MODE")
ENDIF (AURORA_PROFILE_MODE)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (CMRC)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FILE(GLOB_RECURSE PROJECT_RESOURCE "Resources/*")

FetchContent_Declare(External_CMRC
        GIT_REPOSITORY https://github.com/vector-of-bool/cmrc)
FetchContent_MakeAvailable(External_CMRC)

INCLUDE(${external_cmrc_SOURCE_DIR}/CMakeRC.cmake)
CMRC_ADD_RESOURCE_LIBRARY(Engine-Resources
        ALIAS Engine::RC NAMESPACE Resources WHENCE "${CMAKE_CURRENT_SOURCE_DIR}/Resources" ${PROJECT_RESOURCE})

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (SDL)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SET(SDL_SHARED             OFF CACHE BOOL "" FORCE)
SET(SDL_STATIC             ON  CACHE BOOL "" FORCE)
SET(SDL_TEST_LIBRARY       OFF CACHE BOOL "" FORCE)
SET(SDL_DISABLE_UNINSTALL  ON  CACHE BOOL "" FORCE)
SET(SDL_AUDIO_DEFAULT      ON  CACHE BOOL "" FORCE)
SET(SDL_VIDEO_DEFAULT      ON  CACHE BOOL "" FORCE)
SET(SDL_JOYSTICK_DEFAULT   OFF CACHE BOOL "" FORCE)
SET(SDL_HAPTIC_DEFAULT     OFF CACHE BOOL "" FORCE)
SET(SDL_HIDAPI_DEFAULT     ON  CACHE BOOL "" FORCE)
SET(SDL_POWER_DEFAULT      OFF CACHE BOOL "" FORCE)
SET(SDL_SENSOR_DEFAULT     OFF CACHE BOOL "" FORCE)
SET(SDL_DIALOG_DEFAULT     OFF CACHE BOOL "" FORCE)
SET(SDL_CAMERA_DEFAULT     OFF CACHE BOOL "" FORCE)

FetchContent_Declare(External_SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG main)
FetchContent_MakeAvailable(External_SDL)

LIST(APPEND PROJECT_DEPENDENCIES "SDL3-static")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (FAudio)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SET(XNASONG            OFF                                CACHE BOOL   "" FORCE)
SET(BUILD_SHARED_LIBS  OFF                                CACHE BOOL   "" FORCE)
SET(SDL3_INCLUDE_DIRS  ${external_sdl_SOURCE_DIR}/include CACHE STRING "" FORCE)
SET(SDL3_LIBRARIES     "SDL3-static"                      CACHE STRING "" FORCE)

FetchContent_Declare(External_FAudio
        GIT_REPOSITORY https://github.com/FNA-XNA/FAudio
        EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(External_FAudio)

INSTALL(TARGETS SDL3-static  EXPORT FAudio-targets-static)
INSTALL(TARGETS SDL3_Headers EXPORT FAudio-targets-static)

LIST(APPEND PROJECT_DEPENDENCIES "FAudio-static")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (in-drop replacement for unordered_map and unordered_set)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_Ankerl
        GIT_REPOSITORY https://github.com/martinus/unordered_dense
        GIT_TAG main)
FetchContent_MakeAvailable(External_Ankerl)

ADD_LIBRARY(Ankerl INTERFACE)
TARGET_INCLUDE_DIRECTORIES(Ankerl INTERFACE ${external_ankerl_SOURCE_DIR}/include/ankerl)

LIST(APPEND PROJECT_DEPENDENCIES "Ankerl")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (in-drop replacement for array)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_Inplace_Vector
        GIT_REPOSITORY https://github.com/bemanproject/inplace_vector
        GIT_TAG main)
FetchContent_MakeAvailable(External_Inplace_Vector)

LIST(APPEND PROJECT_DEPENDENCIES "beman::inplace_vector")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (Enumeration Reflection)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_Enchantum
        GIT_REPOSITORY https://github.com/ZXShady/enchantum
        GIT_TAG main)
FetchContent_MakeAvailable(External_Enchantum)

LIST(APPEND PROJECT_DEPENDENCIES "enchantum")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (Logger)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SET(QUILL_NO_THREAD_NAME_SUPPORT      ON CACHE BOOL "" FORCE)
SET(QUILL_DISABLE_NON_PREFIXED_MACROS ON CACHE BOOL "" FORCE)

ADD_COMPILE_DEFINITIONS(
        $<$<CONFIG:Debug>:QUILL_COMPILE_ACTIVE_LOG_LEVEL=QUILL_COMPILE_ACTIVE_LOG_LEVEL_DEBUG>
        $<$<NOT:$<CONFIG:Debug>>:QUILL_COMPILE_ACTIVE_LOG_LEVEL=QUILL_COMPILE_ACTIVE_LOG_LEVEL_INFO>)

FetchContent_Declare(External_Quill
        GIT_REPOSITORY https://github.com/odygrd/quill
        GIT_TAG master)
FetchContent_MakeAvailable(External_Quill)

LIST(APPEND PROJECT_DEPENDENCIES "quill")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (Flecs ECS)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

OPTION(FLECS_REST_SERVICE "Enable Flecs rest service" OFF)

SET(FLECS_STATIC  ON  CACHE BOOL "" FORCE)
SET(FLECS_SHARED  OFF CACHE BOOL "" FORCE)

LIST(APPEND PROJECT_DEFINES
        FLECS_CUSTOM_BUILD
        FLECS_CPP
        FLECS_DOC
        FLECS_OS_API_IMPL
        FLECS_PIPELINE
        FLECS_SYSTEM
        FLECS_HTTP
        FLECS_REST
        FLECS_LOG
        FLECS_STATS)

IF    (CMAKE_BUILD_TYPE MATCHES DEBUG)
    LIST(APPEND PROJECT_DEFINES FLECS_LOG)
ENDIF (CMAKE_BUILD_TYPE MATCHES DEBUG)

IF    (FLECS_REST_SERVICE)
    LIST(APPEND PROJECT_DEFINES FLECS_HTTP FLECS_REST FLECS_STATS)

    ADD_COMPILE_DEFINITIONS(FLECS_REST_SERVICE)
ENDIF (FLECS_REST_SERVICE)

FetchContent_Declare(External_Flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs)
FetchContent_MakeAvailable(External_Flecs)

LIST(APPEND PROJECT_DEPENDENCIES "flecs_static")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (TOML)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ADD_COMPILE_DEFINITIONS(TOML_EXCEPTIONS=0)

FetchContent_Declare(External_TOML
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus)
FetchContent_MakeAvailable(External_TOML)

ADD_LIBRARY(Toml INTERFACE)
TARGET_INCLUDE_DIRECTORIES(Toml INTERFACE ${external_toml_SOURCE_DIR})

LIST(APPEND PROJECT_DEPENDENCIES "Toml")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (BMP / PNG) -> TODO: Remove
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LIST(APPEND PROJECT_DEFINES "STBI_NO_PSD")
LIST(APPEND PROJECT_DEFINES "STBI_NO_TGA")
LIST(APPEND PROJECT_DEFINES "STBI_NO_GIF")
LIST(APPEND PROJECT_DEFINES "STBI_NO_HDR")
LIST(APPEND PROJECT_DEFINES "STBI_NO_PIC")
LIST(APPEND PROJECT_DEFINES "STBI_NO_PNM")
LIST(APPEND PROJECT_DEFINES "STBI_NO_STDIO")
LIST(APPEND PROJECT_DEFINES "STB_IMAGE_STATIC")

FetchContent_Declare(External_STB
        GIT_REPOSITORY https://github.com/nothings/stb)
FetchContent_MakeAvailable(External_STB)

ADD_LIBRARY(STB INTERFACE)
TARGET_INCLUDE_DIRECTORIES(STB INTERFACE ${external_stb_SOURCE_DIR})

LIST(APPEND PROJECT_DEPENDENCIES "STB")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (WAV / MP3) -> TODO: Remove
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LIST(APPEND PROJECT_DEFINES "DR_MP3_FLOAT_OUTPUT")
LIST(APPEND PROJECT_DEFINES "DR_MP3_NO_STDIO")
LIST(APPEND PROJECT_DEFINES "DR_WAV_NO_STDIO")

FetchContent_Declare(External_DRLIBS
        GIT_REPOSITORY https://github.com/mackron/dr_libs)
FetchContent_MakeAvailable(External_DRLIBS)

ADD_LIBRARY(DrLibs INTERFACE)
TARGET_INCLUDE_DIRECTORIES(DrLibs INTERFACE ${external_drlibs_SOURCE_DIR})

LIST(APPEND PROJECT_DEPENDENCIES "DrLibs")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (GLTF) -> TODO: Remove
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ADD_COMPILE_DEFINITIONS(TINYGLTF_NO_STB_IMAGE_WRITE)

FetchContent_Declare(External_GLTF
        GIT_REPOSITORY https://github.com/syoyo/tinygltf
        GIT_TAG release)
FetchContent_MakeAvailable(External_GLTF)

ADD_LIBRARY(GLTF INTERFACE)
TARGET_INCLUDE_DIRECTORIES(GLTF INTERFACE ${external_gltf_SOURCE_DIR})

LIST(APPEND PROJECT_DEPENDENCIES "GLTF")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (Tracy) -> TODO: Remove
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SET(TRACY_ENABLE     ON  CACHE BOOL "" FORCE)
SET(TRACY_ON_DEMAND  OFF CACHE BOOL "" FORCE)

FetchContent_Declare(External_Tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy)
FetchContent_MakeAvailable(External_Tracy)

LIST(APPEND PROJECT_DEPENDENCIES "TracyClient")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Library
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE})

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Definitions
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PRIVATE ${PROJECT_DEFINES})

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Libraries
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${PROJECT_DEPENDENCIES} PRIVATE Engine::RC)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Includes
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE})

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## PCH
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TARGET_PRECOMPILE_HEADERS(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Public/Aurora.Base/Base.hpp)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Properties
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TARGET_COMPILE_FEATURES(${PROJECT_NAME} PUBLIC cxx_std_20)

IF    (WIN32)

    TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0601)
    TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)

ENDIF (WIN32)

IF(CMAKE_BUILD_TYPE STREQUAL "Release") # Release-specific options
    IF    (MSVC)
        TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE /O2 /fp:fast /Oy /Ob2 /Gw /GL /arch:AVX2)

        TARGET_LINK_OPTIONS(${PROJECT_NAME} PRIVATE /LTCG)
    ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE -O3 -ffast-math -funroll-loops -fomit-frame-pointer -march=native)

        TARGET_LINK_OPTIONS(${PROJECT_NAME} PRIVATE -flto)
    ENDIF (MSVC)
ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Debug") # Debug-specific options
    IF    (MSVC)
        TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE /Od /RTC1)
    ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE -g3 -O0)
    ENDIF (MSVC)
ENDIF()
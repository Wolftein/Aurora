## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
##
## This work is licensed under the terms of the MIT license.
##
## For a copy, see <https://opensource.org/licenses/MIT>.
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Project
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

PROJECT(Aurora_Engine)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Code
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FILE(GLOB_RECURSE PROJECT_SOURCE "Public/*.cpp" "Private/*.cpp")
FILE(GLOB_RECURSE PROJECT_RESOURCE "Resources/*")

IF    (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    LIST(FILTER PROJECT_SOURCE EXCLUDE REGEX ".*D3D11.*")
ENDIF ()

IF    (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    LIST(FILTER PROJECT_SOURCE EXCLUDE REGEX ".*Asio.*") ## ASIO is not currently supported yet in Emscripten
ENDIF ()

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Includes
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LIST(APPEND PROJECT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Public ${CMAKE_CURRENT_SOURCE_DIR}/Private)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (CMRC)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_CMRC
        GIT_REPOSITORY https://github.com/vector-of-bool/cmrc)
FetchContent_MakeAvailable(External_CMRC)

INCLUDE(${external_cmrc_SOURCE_DIR}/CMakeRC.cmake)
CMRC_ADD_RESOURCE_LIBRARY(Engine-Resources
        ALIAS Engine::RC NAMESPACE Resources WHENCE ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${PROJECT_RESOURCE})

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
SET(BUILD_SHARED_LIBS, OFF                                CACHE BOOL   "" FORCE)
SET(BUILD_SDL3         ON                                 CACHE BOOL   "" FORCE)
SET(SDL3_INCLUDE_DIRS  ${external_sdl_SOURCE_DIR}/include CACHE STRING "" FORCE)
SET(SDL3_LIBRARIES     "SDL3-static"                      CACHE STRING "" FORCE)

FetchContent_Declare(External_FAudio
        GIT_REPOSITORY https://github.com/Wolftein/FAudio
        EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(External_FAudio)

LIST(APPEND PROJECT_DEPENDENCIES "FAudio-static")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (ASIO)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_ASIO
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio)
FetchContent_MakeAvailable(External_ASIO)

ADD_LIBRARY(ASIO INTERFACE)
TARGET_INCLUDE_DIRECTORIES(ASIO INTERFACE ${external_asio_SOURCE_DIR}/asio/include)

LIST(APPEND PROJECT_DEPENDENCIES "ASIO")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (ECS)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SET(FLECS_STATIC  ON  CACHE BOOL "" FORCE)
SET(FLECS_SHARED  OFF CACHE BOOL "" FORCE)

ADD_DEFINITIONS(-DFLECS_CUSTOM_BUILD)
ADD_DEFINITIONS(-DFLECS_CPP)
ADD_DEFINITIONS(-DFLECS_CPP_NO_ENUM_REFLECTION)
ADD_DEFINITIONS(-DFLECS_DOC)
ADD_DEFINITIONS(-DFLECS_LOG)
ADD_DEFINITIONS(-DFLECS_META)
ADD_DEFINITIONS(-DFLECS_OS_API_IMPL)
ADD_DEFINITIONS(-DFLECS_PIPELINE)
ADD_DEFINITIONS(-DFLECS_SYSTEM)

FetchContent_Declare(External_Flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs)
FetchContent_MakeAvailable(External_Flecs)

LIST(APPEND PROJECT_DEPENDENCIES "flecs_static")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (in-drop replacement for unordered_map)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_Ankerl
        GIT_REPOSITORY https://github.com/martinus/unordered_dense
        GIT_TAG main)
FetchContent_MakeAvailable(External_Ankerl)

ADD_LIBRARY(Ankerl INTERFACE)
TARGET_INCLUDE_DIRECTORIES(Ankerl INTERFACE ${external_ankerl_SOURCE_DIR}/include/ankerl)

LIST(APPEND PROJECT_DEPENDENCIES "Ankerl")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (BMP / PNG) -> TODO: Remove
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ADD_DEFINITIONS(-DSTBI_NO_PSD)
ADD_DEFINITIONS(-DSTBI_NO_TGA)
ADD_DEFINITIONS(-DSTBI_NO_GIF)
ADD_DEFINITIONS(-DSTBI_NO_HDR)
ADD_DEFINITIONS(-DSTBI_NO_PIC)
ADD_DEFINITIONS(-DSTBI_NO_PNM)
ADD_DEFINITIONS(-DSTBI_NO_STDIO)

FetchContent_Declare(External_STB
        GIT_REPOSITORY https://github.com/nothings/stb)
FetchContent_MakeAvailable(External_STB)

ADD_LIBRARY(STB INTERFACE)
TARGET_INCLUDE_DIRECTORIES(STB INTERFACE ${external_stb_SOURCE_DIR})

LIST(APPEND PROJECT_DEPENDENCIES "STB")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (WAV / MP3) -> TODO: Remove
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_DRLIBS
        GIT_REPOSITORY https://github.com/mackron/dr_libs)
FetchContent_MakeAvailable(External_DRLIBS)

ADD_LIBRARY(DrLibs INTERFACE)
TARGET_INCLUDE_DIRECTORIES(DrLibs INTERFACE ${external_drlibs_SOURCE_DIR})

LIST(APPEND PROJECT_DEPENDENCIES "DrLibs")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (GLTF) -> TODO: Remove
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_GLTF
        GIT_REPOSITORY https://github.com/syoyo/tinygltf
        GIT_TAG release)
FetchContent_MakeAvailable(External_GLTF)

ADD_LIBRARY(GLTF INTERFACE)
TARGET_INCLUDE_DIRECTORIES(GLTF INTERFACE ${external_gltf_SOURCE_DIR})

LIST(APPEND PROJECT_DEPENDENCIES "GLTF")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (TOML) -> TODO: Remove
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_TOML
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus)
FetchContent_MakeAvailable(External_TOML)

ADD_LIBRARY(Toml INTERFACE)
TARGET_INCLUDE_DIRECTORIES(Toml INTERFACE ${external_toml_SOURCE_DIR})

LIST(APPEND PROJECT_DEPENDENCIES "Toml")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (SPDLOG) -> TODO: Remove
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SET(BUILD_SHARED_LIBS     OFF)
SET(SPDLOG_BUILD_SHARED   OFF)
SET(SPDLOG_USE_STD_FORMAT ON)

FetchContent_Declare(External_SPDLOG
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.x)
FetchContent_MakeAvailable(External_SPDLOG)

LIST(APPEND PROJECT_DEPENDENCIES "spdlog::spdlog")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (Enumeration static reflection) -> TODO: REMOVE
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FetchContent_Declare(External_Neargye
        GIT_REPOSITORY https://github.com/Neargye/magic_enum
        GIT_TAG master)
FetchContent_MakeAvailable(External_Neargye)

ADD_LIBRARY(Neargye INTERFACE)
TARGET_INCLUDE_DIRECTORIES(Neargye INTERFACE ${external_neargye_SOURCE_DIR}/include)

LIST(APPEND PROJECT_DEPENDENCIES "Neargye")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Configuration
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ADD_DEFINITIONS(-DAE_CONTENT_LOADER_MP3)        # .mp3
ADD_DEFINITIONS(-DAE_CONTENT_LOADER_WAV)        # .wav
ADD_DEFINITIONS(-DAE_CONTENT_LOADER_STB)        # .png, .bmp, .tga, .jpg
ADD_DEFINITIONS(-DAE_CONTENT_LOADER_EFFECT)     # .effect, .shader
ADD_DEFINITIONS(-DAE_CONTENT_LOADER_ARTERY)     # .arfont
ADD_DEFINITIONS(-DAE_CONTENT_LOADER_MODEL)      # .gltf

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Library
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE})

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Libraries
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${PROJECT_DEPENDENCIES} PRIVATE Engine::RC)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Includes
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE})
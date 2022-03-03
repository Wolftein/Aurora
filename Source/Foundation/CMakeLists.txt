## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Copyright (C) 2021 by Agustin Alvarez. All rights reserved.
##
## This work is licensed under the terms of the MIT license.
##
## For a copy, see <https://opensource.org/licenses/MIT>.
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

PROJECT(Engine)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Code
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

FILE(GLOB_RECURSE PROJECT_SOURCE "Public/*.cpp" "Private/*.cpp")

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LIST(APPEND PROJECT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/Public ${CMAKE_CURRENT_SOURCE_DIR}/Private)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (XAudio2 (Win32)) TODO REMOVE
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

LIST(APPEND PROJECT_LIBRARY xaudio2)
LIST(APPEND PROJECT_LIBRARY dxgi)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (EASTL in-drop replacement for STL)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ExternalProject_Add(EP_EASTL
        GIT_REPOSITORY https://github.com/electronicarts/EASTL
        GIT_SUBMODULES_RECURSE OFF
        CMAKE_ARGS
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
            -DEASTL_BUILD_TESTS=OFF
            -DEASTL_BUILD_BENCHMARK=OFF
        UPDATE_COMMAND "" INSTALL_COMMAND "")
ExternalProject_Get_Property(EP_EASTL SOURCE_DIR)
ExternalProject_Get_Property(EP_EASTL BINARY_DIR)
LIST(APPEND PROJECT_INCLUDE
        ${SOURCE_DIR}/include
        ${SOURCE_DIR}/test/packages/EAAssert/include
        ${SOURCE_DIR}/test/packages/EABase/include/common
        ${SOURCE_DIR}/test/packages/EAMain/include
        ${SOURCE_DIR}/test/packages/EAStdC/include
        ${SOURCE_DIR}/test/packages/EAThread/include)
LIST(APPEND PROJECT_LIBRARY
        ${BINARY_DIR}${CONFIGURATION_SUFFIX}/EASTL.lib)

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Dependency (GLFW)
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ExternalProject_Add(EP_GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_SUBMODULES_RECURSE OFF
        CMAKE_ARGS
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        UPDATE_COMMAND "" INSTALL_COMMAND "")
ExternalProject_Get_Property(EP_GLFW SOURCE_DIR)
ExternalProject_Get_Property(EP_GLFW BINARY_DIR)
LIST(APPEND PROJECT_INCLUDE ${SOURCE_DIR}/include)
LIST(APPEND PROJECT_LIBRARY ${BINARY_DIR}${CONFIGURATION_SUFFIX}/src/GLFW3.lib)

IF    (WIN32)
    ADD_DEFINITIONS(-DGLFW_EXPOSE_NATIVE_WIN32)
ENDIF ()

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Library
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

ADD_LIBRARY(${PROJECT_NAME} STATIC ${PROJECT_SOURCE})

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Libraries
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARY})

## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
## Includes
## -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE})
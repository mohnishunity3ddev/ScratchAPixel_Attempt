cmake_minimum_required(VERSION 3.22.0)

project(ScratchAPixel CXX C)

set(EXTERN_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external)

set(CMAKE_CXX_STANDARD 20) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Definitions
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

option(USE_DEBUG "Enter debug mode" OFF)

if(USE_DEBUG)
  add_definitions(-DDEBUG)
endif()

add_definitions(-DROW_MAJOR)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include Common
include_directories(common)
add_subdirectory(common)

add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.01.Geometry)
add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.03.Interpolation)
add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.02.matrix.inverse.gauss.jordan.method)
add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.04.camera.lookAt.function)

add_subdirectory(src/Chapter02.3D_Rendering_For_Beginners/02.1.IntroToRayTracing)

add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.05.mathematics.for.shading)
add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.06.scratch)

add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.07.monte-carlo)

add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.08.inverse-transform-sampling)

add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.09.monte-carlo-simulation)

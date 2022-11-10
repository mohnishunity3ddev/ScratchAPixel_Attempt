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
if (USE_DEBUG)
  add_definitions(-DDEBUG)
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include Common
include_directories(common)
add_subdirectory(common)


add_subdirectory(src/Chapter01.Maths.For.Computer.Graphics/01.Geometry)
add_subdirectory(src/Chapter02.3D_Rendering_For_Beginners/02.1.IntroToRayTracing)
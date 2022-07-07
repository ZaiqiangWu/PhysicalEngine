cmake_minimum_required(VERSION 3.10)

project(PhysicalEngine)

find_package( OpenGL REQUIRED )

include_directories(${OpenGL_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-DPROJECT_DIR="${CMAKE_SOURCE_DIR}")

# use glfw
set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/glfwlib) # give hint to cmake to find glfw library
find_package(glfw3 REQUIRED)

# use assimp
set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/assimplib) # give hint to cmake to find glfw library
find_package(assimp REQUIRED)

# use freetype
#set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/freetypelib) # give hint to cmake to find glfw library
#set(FREETYPE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/freetypelib)
find_package(freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(/usr/include/freetype2)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

include_directories(src/include)
include_directories(src/)
include_directories(src/object)
include_directories(src/simulation)
include_directories(src/SOIL)
include_directories(${ASSIMP_INCLUDE_DIRS})

#include_directories(external/assimp)
#include_directories(${PROJECT_SOURCE_DIR}/external/delfem2/include)
include_directories(${PROJECT_SOURCE_DIR}/external)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(source_code src/main.cpp src/shader.cpp)
add_subdirectory(src/SOIL)


#option(USE_HEADERONLY "USE_DFM2_HEADERONLY" ON)
add_library(cnpy external/cnpy/cnpy.cpp)

add_executable(main ${source_code} )
if(WIN32)
target_link_libraries(main  OpenGL::GL glew32s.lib SOILLIB  glfw ${ASSIMP_LIBRARIES} ${ZLIB_LIBRARIES} ${OPENDDL_PARSER_LIBRARIES} ${FREETYPE_LIBRARIES} cnpy)
elseif(UNIX)
target_link_libraries(main  GL GLEW SOILLIB  glfw ${ASSIMP_LIBRARIES} ${ZLIB_LIBRARIES} ${OPENDDL_PARSER_LIBRARIES} ${FREETYPE_LIBRARIES} cnpy)
endif()
#opengl32.lib glew32s.lib freetype.lib
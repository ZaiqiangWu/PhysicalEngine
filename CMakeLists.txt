cmake_minimum_required(VERSION 3.10)

project(PhysicalEngine)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-DUNICODE -D_UNICODE)

include_directories(src/include)
include_directories(src/SOIL)
include_directories(external/stb/include)
include_directories(${PROJECT_SOURCE_DIR}/external/tinyobjloader/include)
include_directories(${PROJECT_SOURCE_DIR}/external/tinyxfileloader/include)
include_directories(external/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/test_inputs)
include_directories(${PROJECT_SOURCE_DIR}/external)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
list(APPEND garment_code 
    external/cnpy/cnpy.h                    
    external/cnpy/cnpy.cpp
    test_inputs/inputs_garment.h
)
add_definitions(-DDFM2_HEADER_ONLY=ON)  
set(source_code src/main.cpp src/shader.cpp)

add_library(Third_Party SHARED IMPORTED)
add_subdirectory(src/SOIL)
add_subdirectory(src/include/Saba)

#set(soil_code src/SOIL/SOIL.c src/SOIL/image_DXT.c src/SOIL/image_helper.c src/SOIL/stb_image_aug.c src/SOIL/stbi_DDS_aug_c.h)

#add_library(SOILLIB ${soil_code})

#add_library(SDL2 lib/freeglut.dll lib/glew64.dll)
#add_library(SOIL ${soil_code})
#message(${SOILLIB})
add_library(cnpy external/cnpy/cnpy.cpp)
add_executable(main ${source_code} )
target_link_libraries(main  opengl32.lib glew32s.lib SOILLIB  glfw3.lib assimp-vc140-mt.lib Saba cnpy ${ZLIB_LIBRARIES})
set(output_path ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
configure_file(lib/assimp-vc140-mt.dll ${CMAKE_BINARY_DIR} COPYONLY)
configure_file(lib/assimp-vc140-mt.dll ${output_path} COPYONLY)

#configure_file(${CMAKE_BINARY_DIR}/Release/main.exe ${output_path} COPYONLY)

#configure_file(lib/glew64.dll ${CMAKE_BINARY_DIR} COPYONLY)
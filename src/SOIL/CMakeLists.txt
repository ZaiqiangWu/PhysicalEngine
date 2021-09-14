cmake_minimum_required(VERSION 3.10)

project(SOIL)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(soil_code SOIL.c image_DXT.c image_helper.c stb_image_aug.c)

add_library(SOILLIB ${soil_code})


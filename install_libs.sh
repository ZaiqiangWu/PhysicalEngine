#!/bin/bash
git submodule update --init external/glfw
cd external/glfw
cmake .
cmake --build . --config Release
cmake --install . --prefix ../glfwlib
cd ..
cd ..

git submodule update --init external/assimp
cd external/assimp
cmake . -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DBUILD_SHARED_LIBS=OFF
cmake --build . --config Release
cmake --install . --prefix ../assimplib

cd ..
cd ..


# Game Engine
A light-weight game engine

## How to build glfw
```
git submodule update --init external/glfw
cd external/glfw
cmake .
cmake --build . --config Release
cmake --install . --prefix ../glfwlib
```

## Build assimp
```
git submodule update --init external/assimp
cd external/assimp
cmake . -DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DBUILD_SHARED_LIBS=OFF
cmake --build . --config Release
cmake --install . --prefix ../assimplib
```

## Build freetype
```
git submodule update --init external/freetype
cd external/freetype
mkdir build && cd build
cmake .. 
cmake --build . --config Release
cmake --install . --prefix ../../freetypelib
```

## build the engine
```
mkdir build && cd build
cmake ..
make
```
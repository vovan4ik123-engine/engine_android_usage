cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-fexceptions -frtti)

# for info/warning/assert messages
add_definitions(-DBR_DEBUG)

add_library(main SHARED)
target_sources(main PRIVATE main.cpp PlayGUILayer.cpp Play3DSceneLayer.cpp PauseGUILayer.cpp)

#all other desired libs should be included in Beryll-static lib
target_link_libraries(main Beryll-static GLESv3)

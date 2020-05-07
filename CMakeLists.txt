cmake_minimum_required (VERSION 3.10)
project(Shadertoy)

set(CMAKE_CXX_STANDARD 17)

#Libraries
add_subdirectory(3rdparty/glad)
add_subdirectory(3rdparty/SDL)

#Necessary to make includes work correctly
include_directories(include)
include_directories(include/3d)
include_directories(include/gl)
include_directories(include/math)
include_directories(include/utils)

#Find Sources and Headers in src folder
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

#Test executable
add_executable(Shadertoy ${SOURCES} ${HEADERS})
target_include_directories(Shadertoy PRIVATE 3rdparty/SDL/include 3rdparty/stb)
target_link_libraries(Shadertoy SDL2 glad)
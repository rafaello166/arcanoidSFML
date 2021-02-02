cmake_minimum_required(VERSION 3.1)

project(SFMLTest)

## If you want to link SFML statically
 set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
 set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(SFMLTest main.cpp Ball.cpp Ball.h Paddle.cpp Paddle.h Block.cpp Block.h Menu.cpp Menu.h)
target_link_libraries(SFMLTest sfml-graphics sfml-audio)
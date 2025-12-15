CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project("rainfall")

add_subdirectory(lib)
add_subdirectory(game)
add_subdirectory(editor)

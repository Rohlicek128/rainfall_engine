file(GLOB_RECURSE SRCS "src/*.cpp")

list(APPEND SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/glad.c"
)


add_library(engine STATIC ${SRCS})
TARGET_INCLUDE_DIRECTORIES(engine
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/stb"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/yaml-cpp"
)

target_link_libraries(engine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/glfw3.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/yaml-cpp/yaml-cppd.lib"
    "opengl32"
)

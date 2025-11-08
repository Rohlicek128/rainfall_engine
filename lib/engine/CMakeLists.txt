file(GLOB_RECURSE SRCS "src/*.cpp")

list(APPEND SRCS "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/glad.c")


add_library(engine STATIC ${SRCS})
TARGET_INCLUDE_DIRECTORIES(engine
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

target_link_libraries(engine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/glfw3.lib" "opengl32")

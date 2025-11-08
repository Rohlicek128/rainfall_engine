INCLUDE_DIRECTORIES("../lib")

FILE(GLOB_RECURSE SRCS "src/*.cpp")
ADD_EXECUTABLE("game" ${SRCS})

TARGET_INCLUDE_DIRECTORIES("game" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries("game" "engine")

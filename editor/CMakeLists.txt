INCLUDE_DIRECTORIES("../lib")

FILE(GLOB_RECURSE SRCS "src/*.cpp")
ADD_EXECUTABLE(editor ${SRCS})

TARGET_INCLUDE_DIRECTORIES(editor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(editor PRIVATE engine)

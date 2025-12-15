INCLUDE_DIRECTORIES("../lib")

FILE(GLOB_RECURSE SRCS "src/*.cpp")
ADD_EXECUTABLE("game" ${SRCS})

TARGET_INCLUDE_DIRECTORIES("game" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries("game" PRIVATE "engine")

add_custom_command(
    TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/game/assets"
        "$<TARGET_FILE_DIR:game>/assets"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/game/saved"
        "$<TARGET_FILE_DIR:game>/saved"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib/engine/src/Rendering/Shaders/GLSL"
        "$<TARGET_FILE_DIR:game>/engine/shaders"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib/engine/assets"
        "$<TARGET_FILE_DIR:game>/engine/assets"
)

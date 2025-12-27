INCLUDE_DIRECTORIES("../lib")

FILE(GLOB_RECURSE SRCS "src/*.cpp")
LIST(APPEND SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/ImGuizmo.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_impl_opengl3.cpp"
)

ADD_EXECUTABLE(editor ${SRCS})

TARGET_INCLUDE_DIRECTORIES(editor
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui"
)

target_link_libraries(editor PRIVATE engine)

add_custom_command(
    TARGET editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib/engine/src/Rendering/Shaders/GLSL"
        "$<TARGET_FILE_DIR:editor>/engine/shaders"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/lib/engine/assets"
        "$<TARGET_FILE_DIR:editor>/engine/assets"
)

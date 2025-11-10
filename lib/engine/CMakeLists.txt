file(GLOB_RECURSE SRCS "src/*.cpp")

list(APPEND SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/glad.c"

    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/ImGuizmo.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui/imgui_impl_opengl3.cpp"
)


add_library(engine STATIC ${SRCS})
TARGET_INCLUDE_DIRECTORIES(engine
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/Imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/stb"
)

target_link_libraries(engine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/glfw3.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/yaml-cpp/yaml-cppd.lib"
    "opengl32"
)

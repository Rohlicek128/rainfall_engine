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
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/RmlUi"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype"
)

target_compile_definitions(engine PUBLIC RMLUI_STATIC_LIB)

set(RMLUI_BACKEND_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/RmlUi/RmlUi/Backends)
add_library(rmlui_backend_GLFW_GL3 INTERFACE)
target_sources(rmlui_backend_GLFW_GL3 INTERFACE
	${RMLUI_BACKEND_PATH}/RmlUi_Platform_GLFW.cpp
	${RMLUI_BACKEND_PATH}/RmlUi_Renderer_GL3.cpp
)

target_compile_definitions(engine PRIVATE RMLUI_STATIC_LIB)
target_include_directories(rmlui_backend_GLFW_GL3 INTERFACE ${RMLUI_BACKEND_PATH})
target_compile_definitions(rmlui_backend_GLFW_GL3 INTERFACE
	RMLUI_NUM_MSAA_SAMPLES=4
	RMLUI_GL3_CUSTOM_LOADER=<glad.h>
)

target_link_libraries(engine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/glfw3.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/yaml-cpp/yaml-cppd.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/RmlUi/rmlui_debugger.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/RmlUi/rmlui.lib"
    rmlui_backend_GLFW_GL3
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/freetype/freetyped.lib"
    "opengl32"
    "dwmapi"
)

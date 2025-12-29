#pragma once

struct GLFWwindow;
class ImGuiIO;

class Framebuffer;

namespace editor
{
    class ImGuiLayer
    {
        void set_red_style();

    public:
        ImGuiIO* imgui_io;

        ImGuiLayer();

        void init(GLFWwindow* window);
        void shutdown();

        void on_begin_frame();
        void on_end_frame();
    };
}

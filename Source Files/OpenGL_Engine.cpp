#include "Rendering/Engine.h"
#include "Rendering/Window.h"

int main()
{
    Window window("Rainfall Engine", 1920, 1080);
    Engine engine(Window::engine_args);

    window.run(engine);
    
    return 0;
}
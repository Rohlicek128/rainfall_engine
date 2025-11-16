#include <engine/src/Rendering/Window.h>

#include <string>
#include <utils.h>

int main()
{
	game::tools::printl_message("GAME", "Start");

	engine::Window window("Rainfall Engine", 1440, 1024);
	Engine engine(engine::Window::engine_args);
	window.run(engine);

	game::tools::printl_message("GAME", "End");

	return 0;
}

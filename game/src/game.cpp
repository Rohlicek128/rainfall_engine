#include <engine/src/Rendering/Window.h>

#include <string>
#include <test.h>
#include <other.h>
#include <utils.h>

int main()
{
    namespace gt = game::tools;

	game::tools::printl_message("Hello CMake.");
	gt::printl_message(game::Testing::get_message());
	gt::printl_message(game::Another::get_message());


	engine::Window window("GAME", 1440, 1024);
	//window.run();


	game::tools::printl_message("END");
	return 0;
}

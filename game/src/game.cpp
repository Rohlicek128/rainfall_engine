#include "engine/src/render.h"

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

	engine::Renderer r;
	r.init_window("GAME TESTING", 1440, 1024);

	while (true)
	{
		r.render();
	}
	r.terminate();

	game::tools::printl_message("Frames: " + std::to_string(r.get_frame_count()));
	return 0;
}

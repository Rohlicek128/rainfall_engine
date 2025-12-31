#include <engine/core/Engine.h>

#include <utils.h>
#include <string>

#include "ExampleGame.h"


int main()
{
	game::tools::printl_message("GAME", "Start");

	engine::run<game::ExampleGame>();

	game::tools::printl_message("GAME", "End");
	return 0;
}

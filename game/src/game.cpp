#include <engine/rendering/Window.h>
#include <engine/rendering/Renderer.h>
#include <engine/world/Scene.h>
#include <engine/src/Rendering/Entities/Entity.h>
#include <engine/src/Rendering/World/Mesh.h>
#include <engine/src/Rendering/Buffers/Screen/GBuffer.h>

#include <utils.h>
#include <string>


int main()
{
	game::tools::printl_message("GAME", "Start");


	engine::Window window("Rainfall Engine", 1660, 1080);
	engine::Renderer renderer(window);

	Scene scene("Temp", nullptr);
	scene.load("saved/Example.rain");

	window.run(renderer, scene);


	game::tools::printl_message("GAME", "End");

	return 0;
}

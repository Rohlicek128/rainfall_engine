/*#include <engine/rendering/Window.h>
#include <engine/rendering/Renderer.h>
#include <engine/world/Scene.h>
#include <engine/src/Rendering/Entities/Entity.h>
#include <engine/src/Rendering/World/Mesh.h>
#include <engine/src/Rendering/Buffers/Screen/GBuffer.h>*/

#include <engine/core/Engine.h>

#include <utils.h>
#include <string>

#include "ExampleGame.h"


int main()
{
	game::tools::printl_message("GAME", "Start");

	engine::Run<game::ExampleGame>();


	/*engine::Window window("Rainfall Engine", 1660, 1080);
	engine::Renderer renderer(window);

	Mesh mesh(std::vector<VertexAttribute>{{"Position", 3}, {"TexCoord", 2}, {"Normal", 3}}, true);
	renderer.set_models_to_mesh(&mesh);
	mesh.compile();

	Scene scene("Temp", &mesh);
	scene.load("saved/Example.rain");

	window.run(renderer, scene);*/


	game::tools::printl_message("GAME", "End");

	return 0;
}

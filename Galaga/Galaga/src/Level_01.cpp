#include "Level_01.h"

#include "Commands.h"

using namespace AE;

void Level_01::Load(AE::Scene& scene)
{
	auto go = std::make_shared<AE::GameObject>();
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_RETURN, InputType::IsPressed, std::move(std::make_unique<LoadCommand>(go.get())));

	scene.Add(go);
}

#include "StartScreenLevel.h"

#include "Commands.h"
#include "ImageComponent.h"

using namespace AE;

void StartScreenLevel::Load(AE::Scene& scene)
{
	AddBackgroundImage(scene);

	auto go = std::make_shared<AE::GameObject>();
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_RETURN, InputType::IsUpThisFrame, std::move(std::make_unique<LoadCommand>(go.get())));

	scene.Add(go);
}

void StartScreenLevel::AddBackgroundImage(AE::Scene& scene)
{
	auto go = std::make_shared<AE::GameObject>();
	auto imageComp{ std::make_shared<ImageComponent>(go.get(), "Background.png") };
	imageComp->SetDestRect({ 0, 0, 640, 480 });
	
	go->AddComponent(imageComp);



	scene.Add(go);
}

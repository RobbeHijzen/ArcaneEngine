#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "ArcaneEngine.h"
#include <SDL.h>

#include "TextComponent.h"
#include "ImageComponent.h"
#include "FpsComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "ShootComponent.h"

#include "Observers.h"
#include "Commands.h"


void AddBackgroundImage(Scene& scene);
void AddFPSCounter(Scene& scene);
void AddGalaga(Scene& scene);
void AddControlsExplainers(Scene& scene);

using namespace ObserverPattern;

void LoadScene()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Galaga");

	AddBackgroundImage(scene);
	AddFPSCounter(scene);

	AddGalaga(scene);

	AddControlsExplainers(scene);

}

int main(int, char* [])
{
	ArcaneEngine engine("Resources/");
	engine.Run(LoadScene);

	return 0;
}

// Temp functions to add more overview to load function
void AddBackgroundImage(Scene& scene)
{
	auto backgroundImage = std::make_shared<GameObject>();
	backgroundImage->AddComponent(std::make_shared<ImageComponent>(backgroundImage.get(), "Galaga/GalagaBackground.jpg", 640.f, 480.f));
	scene.Add(backgroundImage);
}

void AddFPSCounter(Scene& scene)
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);
	auto fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent(std::make_shared<FpsComponent>(fpsObject.get()));
	fpsObject->AddComponent(std::make_shared<TextComponent>(fpsObject.get(), "0 FPS", font));
	fpsObject->SetLocalTransform({ 10, 10 });
	scene.Add(fpsObject);

}

void AddGalaga(Scene& scene)
{
	auto galaga = std::make_shared<GameObject>();
	galaga->AddComponent(std::make_shared<ImageComponent>(galaga.get(), "Galaga/GalagaWhite.png", 50.f, 50.f));
	galaga->AddComponent(std::make_shared<HealthComponent>(galaga.get(), 3));
	galaga->AddComponent(std::make_shared<ScoreComponent>(galaga.get()));
	galaga->AddComponent(std::make_shared<ShootComponent>(galaga.get(), glm::vec2{0.f, -1.f}, 300.f));
	galaga->SetLocalTransform({ 200.f, 380.f });

	constexpr float galagaMoveSpeed{ 100.f };
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_A, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ -1.f, 0.f }, galagaMoveSpeed)));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_D, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ 1.f, 0.f }, galagaMoveSpeed)));
	
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ -1.f, 0.f }, galagaMoveSpeed)));
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ 1.f, 0.f }, galagaMoveSpeed)));

	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_C, InputType::IsDownThisFrame, std::move(std::make_unique<KillCommand>(galaga.get())));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_Z, InputType::IsDownThisFrame, std::move(std::make_unique<PickupSilverCommand>(galaga.get())));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_X, InputType::IsDownThisFrame, std::move(std::make_unique<PickupGoldCommand>(galaga.get())));
	
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_SPACE, InputType::IsDownThisFrame, std::move(std::make_unique<ShootCommand>(galaga.get())));

	scene.Add(galaga);


	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 19);

	auto health = std::make_shared<GameObject>();
	auto livesTextComp{ std::make_shared<TextComponent>(health.get(), "#Lives: 3", font) };
	health->AddComponent(livesTextComp);
	health->SetLocalTransform({ 10, 220 });
	scene.Add(health);

	auto score = std::make_shared<GameObject>();
	auto scoreTextComp{ std::make_shared<TextComponent>(score.get(), "#Score: 0", font) };
	score->AddComponent(scoreTextComp);
	score->SetLocalTransform({ 10, 240 });
	scene.Add(score);
	
	galaga->AddObserver(std::move(std::make_unique<HealthDisplayObserver>(livesTextComp.get())));
	galaga->AddObserver(std::move(std::make_unique<PickupObserver>()));
	galaga->AddObserver(std::move(std::make_unique<ScoreDisplayObserver>(scoreTextComp.get())));

}

void AddControlsExplainers(Scene& scene)
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	auto controls1 = std::make_shared<GameObject>();
	controls1->AddComponent(std::make_shared<TextComponent>(controls1.get(),
		"AD: move sideways", font));
	controls1->SetLocalTransform({ 10, 75 });

	auto controls2 = std::make_shared<GameObject>();
	controls2->AddComponent(std::make_shared<TextComponent>(controls2.get(),
		"C: lose Health", font));
	controls2->SetLocalTransform({ 10, 95 });

	auto controls3 = std::make_shared<GameObject>();
	controls3->AddComponent(std::make_shared<TextComponent>(controls3.get(),
		"Z: pick up silver", font));
	controls3->SetLocalTransform({ 10, 115 });

	auto controls4 = std::make_shared<GameObject>();
	controls4->AddComponent(std::make_shared<TextComponent>(controls4.get(),
		"X: pick up gold", font));
	controls4->SetLocalTransform({ 10, 135 });

	auto controls5 = std::make_shared<GameObject>();
	controls5->AddComponent(std::make_shared<TextComponent>(controls5.get(),
		"Space: shoot bullet", font));
	controls5->SetLocalTransform({ 10, 155 });

	scene.Add(controls1);
	scene.Add(controls2);
	scene.Add(controls3);
	scene.Add(controls4);
	scene.Add(controls5);
}

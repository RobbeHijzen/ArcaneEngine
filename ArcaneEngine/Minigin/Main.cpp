#include <SDL.h>


#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "ArcaneEngine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "Observers.h"

#include "TextComponent.h"
#include "ImageComponent.h"
#include "FpsComponent.h"
#include "RotatingComponent.h"


void AddBackgroundImage(Scene& scene);
void AddFPSCounter(Scene& scene);
void AddWhiteGalaga(Scene& scene);
void AddRedGalaga(Scene& scene);
void AddControlsExplainers(Scene& scene);

using namespace ObserverPattern;

void LoadScene()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Galaga");

	AddBackgroundImage(scene);
	AddFPSCounter(scene);

	AddWhiteGalaga(scene);
	AddRedGalaga(scene);


	AddControlsExplainers(scene);

}

int main(int, char* [])
{
	ArcaneEngine engine("../Resources/");
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

void AddWhiteGalaga(Scene& scene)
{
	auto galagaWhite = std::make_shared<GameObject>();
	galagaWhite->AddComponent(std::make_shared<ImageComponent>(galagaWhite.get(), "Galaga/GalagaWhite.png", 50.f, 50.f));
	galagaWhite->AddComponent(std::make_shared<HealthComponent>(galagaWhite.get(), 3));
	galagaWhite->AddComponent(std::make_shared<ScoreComponent>(galagaWhite.get()));
	galagaWhite->SetLocalTransform({ 200.f, 250.f });

	constexpr float GalagaWhiteMoveSpeed{ 100.f };
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_W, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaWhite.get(), glm::vec2{ 0.f, -1.f }, GalagaWhiteMoveSpeed)));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_A, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaWhite.get(), glm::vec2{ -1.f, 0.f }, GalagaWhiteMoveSpeed)));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_S, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaWhite.get(), glm::vec2{ 0.f, 1.f }, GalagaWhiteMoveSpeed)));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_D, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaWhite.get(), glm::vec2{ 1.f, 0.f }, GalagaWhiteMoveSpeed)));

	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_C, InputType::IsDownThisFrame, std::move(std::make_unique<KillCommand>(galagaWhite.get())));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_Z, InputType::IsDownThisFrame, std::move(std::make_unique<IncreaseScoreCommand>(galagaWhite.get(), 10)));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_X, InputType::IsDownThisFrame, std::move(std::make_unique<IncreaseScoreCommand>(galagaWhite.get(), 100)));

	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_UP, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaWhite.get(), glm::vec2{ 0.f, -1.f }, GalagaWhiteMoveSpeed)));
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaWhite.get(), glm::vec2{ -1.f, 0.f }, GalagaWhiteMoveSpeed)));
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaWhite.get(), glm::vec2{ 1.f, 0.f }, GalagaWhiteMoveSpeed)));
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_DOWN, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaWhite.get(), glm::vec2{ 0.f, 1.f }, GalagaWhiteMoveSpeed)));

	scene.Add(galagaWhite);


	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 19);

	auto whiteHealth = std::make_shared<GameObject>();
	auto whiteLivesTextComp{ std::make_shared<TextComponent>(whiteHealth.get(), "#Lives: 3", font) };
	whiteHealth->AddComponent(whiteLivesTextComp);
	whiteHealth->SetLocalTransform({ 10, 190 });
	scene.Add(whiteHealth);

	auto whiteScore = std::make_shared<GameObject>();
	auto whiteScoreTextComp{ std::make_shared<TextComponent>(whiteScore.get(), "#Score: 0", font) };
	whiteScore->AddComponent(whiteScoreTextComp);
	whiteScore->SetLocalTransform({ 10, 210 });
	scene.Add(whiteScore);

	galagaWhite->AddObserver(std::move(std::make_unique<HealthDisplayObserver>(whiteLivesTextComp.get())));
	galagaWhite->AddObserver(std::move(std::make_unique<ScoreDisplayObserver>(whiteScoreTextComp.get())));

}

void AddRedGalaga(Scene& scene)
{
	auto galagaRed = std::make_shared<GameObject>();
	galagaRed->AddComponent(std::make_shared<ImageComponent>(galagaRed.get(), "Galaga/GalagaRed.png", 50.f, 50.f));
	galagaRed->AddComponent(std::make_shared<HealthComponent>(galagaRed.get(), 3));
	galagaRed->AddComponent(std::make_shared<ScoreComponent>(galagaRed.get()));
	galagaRed->SetLocalTransform({ 400.f, 250.f });

	constexpr float galagaRedMoveSpeed{ 200.f };
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_UP, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaRed.get(), glm::vec2{ 0.f, -1.f }, galagaRedMoveSpeed)));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaRed.get(), glm::vec2{ -1.f, 0.f }, galagaRedMoveSpeed)));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaRed.get(), glm::vec2{ 1.f, 0.f }, galagaRedMoveSpeed)));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_DOWN, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galagaRed.get(), glm::vec2{ 0.f, 1.f }, galagaRedMoveSpeed)));

	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_X, InputType::IsDownThisFrame, std::move(std::make_unique<KillCommand>(galagaRed.get())));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_A, InputType::IsDownThisFrame, std::move(std::make_unique<IncreaseScoreCommand>(galagaRed.get(), 10)));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_B, InputType::IsDownThisFrame, std::move(std::make_unique<IncreaseScoreCommand>(galagaRed.get(), 100)));

	scene.Add(galagaRed);



	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 19);

	auto health = std::make_shared<GameObject>();
	auto livesTextComp{ std::make_shared<TextComponent>(health.get(), "#Lives: 3", font) };
	health->AddComponent(livesTextComp);
	health->SetLocalTransform({ 10, 250 });
	scene.Add(health);

	auto score = std::make_shared<GameObject>();
	auto scoreTextComp{ std::make_shared<TextComponent>(score.get(), "#Score: 0", font) };
	score->AddComponent(scoreTextComp);
	score->SetLocalTransform({ 10, 270 });
	scene.Add(score);

	galagaRed->AddObserver(std::move(std::make_unique<HealthDisplayObserver>(livesTextComp.get())));
	galagaRed->AddObserver(std::move(std::make_unique<ScoreDisplayObserver>(scoreTextComp.get())));

}

void AddControlsExplainers(Scene& scene)
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	auto dpadControls = std::make_shared<GameObject>();
	dpadControls->AddComponent(std::make_shared<TextComponent>(dpadControls.get(),
		"Use the D-Pad to move Red Galaga, X kill Red, A and B to gain score", font));
	dpadControls->SetLocalTransform({ 10, 100 });
	scene.Add(dpadControls);


	auto keyboardControls = std::make_shared<GameObject>();
	keyboardControls->AddComponent(std::make_shared<TextComponent>(keyboardControls.get(),
		"Use WASD to move White Galaga, C kill White, Z and X to gain score", font));
	keyboardControls->SetLocalTransform({ 10, 118 });
	scene.Add(keyboardControls);
}

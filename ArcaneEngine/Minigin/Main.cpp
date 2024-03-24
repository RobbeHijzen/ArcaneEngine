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

#include "TextComponent.h"
#include "ImageComponent.h"
#include "FpsComponent.h"
#include "RotatingComponent.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	// Background Image
	auto backgroundImage = std::make_shared<GameObject>();
	backgroundImage->AddComponent(std::make_shared<ImageComponent>(backgroundImage.get(), "Galaga/GalagaBackground.jpg", 640.f, 480.f));
	scene.Add(backgroundImage);
	
	// FPS Counter
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);
	auto fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent(std::make_shared<FpsComponent>(fpsObject.get()));
	fpsObject->AddComponent(std::make_shared<TextComponent>(fpsObject.get(), "0 FPS", font));
	fpsObject->SetLocalTransform({ 10, 10 });
	scene.Add(fpsObject);
	
	
	// Galaga White
	auto galagaWhite = std::make_shared<GameObject>();
	galagaWhite->AddComponent(std::make_shared<ImageComponent>(galagaWhite.get(), "Galaga/GalagaWhite.png", 50.f, 50.f));
	galagaWhite->SetLocalTransform({ 100.f, 250.f });
	
	constexpr float GalagaWhiteMoveSpeed{ 100.f };
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_W, new MoveCommand{ galagaWhite.get(), glm::vec2{0.f, -1.f}, GalagaWhiteMoveSpeed });
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_A, new MoveCommand{ galagaWhite.get(), glm::vec2{-1.f, 0.f}, GalagaWhiteMoveSpeed });
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_S, new MoveCommand{ galagaWhite.get(), glm::vec2{0.f, 1.f}, GalagaWhiteMoveSpeed });
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_D, new MoveCommand{ galagaWhite.get(), glm::vec2{1.f, 0.f}, GalagaWhiteMoveSpeed });
	
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_UP, InputType::IsPressed, new MoveCommand{ galagaWhite.get(), glm::vec2{0.f, -1.f}, GalagaWhiteMoveSpeed });
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsPressed, new MoveCommand{ galagaWhite.get(), glm::vec2{-1.f, 0.f}, GalagaWhiteMoveSpeed });
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsPressed, new MoveCommand{ galagaWhite.get(), glm::vec2{1.f, 0.f}, GalagaWhiteMoveSpeed });
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_DOWN, InputType::IsPressed, new MoveCommand{ galagaWhite.get(), glm::vec2{0.f, 1.f}, GalagaWhiteMoveSpeed });
	
	scene.Add(galagaWhite);
	
	// Galaga Red
	auto galagaRed = std::make_shared<GameObject>();
	galagaRed->AddComponent(std::make_shared<ImageComponent>(galagaRed.get(), "Galaga/GalagaRed.png", 50.f, 50.f));
	galagaRed->SetLocalTransform({150.f, 150.f});

	constexpr float galagaRedMoveSpeed{200.f};
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_UP, InputType::IsPressed, new MoveCommand{ galagaRed.get(), glm::vec2{0.f, -1.f}, galagaRedMoveSpeed });
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsPressed, new MoveCommand{ galagaRed.get(), glm::vec2{-1.f, 0.f}, galagaRedMoveSpeed });
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsPressed, new MoveCommand{ galagaRed.get(), glm::vec2{1.f, 0.f}, galagaRedMoveSpeed });
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_DOWN, InputType::IsPressed, new MoveCommand{ galagaRed.get(), glm::vec2{0.f, 1.f}, galagaRedMoveSpeed });
	
	scene.Add(galagaRed);

}

int main(int, char*[]) {
	ArcaneEngine engine("../Data/");
	engine.Run(load);
    return 0;
}
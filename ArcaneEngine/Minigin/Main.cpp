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
	backgroundImage->AddComponent(std::make_shared<ImageComponent>(backgroundImage.get(), "background.tga"));
	scene.Add(backgroundImage);
	
	// DAE Logo
	auto logo = std::make_shared<GameObject>();
	logo->AddComponent(std::make_shared<ImageComponent>(logo.get(), "logo.tga"));
	logo->SetLocalTransform({ 216, 180 });
	scene.Add(logo);
	
	// The Text
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textObject = std::make_shared<GameObject>();
	textObject->AddComponent(std::make_shared<TextComponent>(textObject.get(), "Programming 4 Assignment", font));
	textObject->SetLocalTransform({ 80, 10 });
	scene.Add(textObject);
	
	// FPS Counter
	auto fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent(std::make_shared<FpsComponent>(fpsObject.get()));
	fpsObject->AddComponent(std::make_shared<TextComponent>(fpsObject.get(), "0 FPS", font));
	fpsObject->SetLocalTransform({ 80, 80 });
	scene.Add(fpsObject);


	// Pacman
	auto pacman = std::make_shared<GameObject>();
	pacman->AddComponent(std::make_shared<ImageComponent>(pacman.get(), "Pacman.png", 50.f, 50.f));
	pacman->AddComponent(std::make_shared<MovementComponent>(pacman.get(), 100.f));
	pacman->SetLocalTransform({ 100.f, 250.f });

	InputManager::GetInstance().BindActionGP(0, XINPUT_GAMEPAD_DPAD_UP, InputType::IsPressed, new MoveCommand{ pacman.get(), glm::vec2{0.f, 1.f}});
	InputManager::GetInstance().BindActionGP(0, XINPUT_GAMEPAD_DPAD_LEFT, InputType::IsPressed, new MoveCommand{ pacman.get(), glm::vec2{-1.f, 0.f}});
	InputManager::GetInstance().BindActionGP(0, XINPUT_GAMEPAD_DPAD_RIGHT, InputType::IsPressed, new MoveCommand{ pacman.get(), glm::vec2{1.f, 0.f}});
	InputManager::GetInstance().BindActionGP(0, XINPUT_GAMEPAD_DPAD_DOWN, InputType::IsPressed, new MoveCommand{ pacman.get(), glm::vec2{0.f, -1.f}});
	scene.Add(pacman);
	
	// Ms. Pacman
	auto msPacman = std::make_shared<GameObject>();
	msPacman->AddComponent(std::make_shared<ImageComponent>(msPacman.get(), "MsPacman.png", 50.f, 50.f));
	msPacman->AddComponent(std::make_shared<MovementComponent>(msPacman.get(), 200.f));
	msPacman->SetLocalTransform({150.f, 150.f});

	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_W, new MoveCommand{ msPacman.get(), glm::vec2{0.f, 1.f} });
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_A, new MoveCommand{ msPacman.get(), glm::vec2{-1.f, 0.f} });
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_S, new MoveCommand{ msPacman.get(), glm::vec2{0.f, -1.f} });
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_D, new MoveCommand{ msPacman.get(), glm::vec2{1.f, 0.f} });
	scene.Add(msPacman);


}

int main(int, char*[]) {
	ArcaneEngine engine("../Data/");
	engine.Run(load);
    return 0;
}
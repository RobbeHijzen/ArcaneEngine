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


	// Pacman ANCHOR
	auto anchor = std::make_shared<GameObject>();
	anchor->SetLocalTransform({ 100, 380 });
	scene.Add(anchor);

	// Rotating Pacman 1
	auto pacman1 = std::make_shared<GameObject>();
	pacman1->AddComponent(std::make_shared<ImageComponent>(pacman1.get(), "pacman.tga"));
	pacman1->AddComponent(std::make_shared<RotatingComponent>(pacman1.get(), 45.f, -3.f));
	scene.Add(pacman1);
	pacman1->SetParent(anchor.get());

	// Rotating Pacman 2
	auto pacman2 = std::make_shared<GameObject>();
	pacman2->AddComponent(std::make_shared<ImageComponent>(pacman2.get(), "pacman.tga"));
	pacman2->AddComponent(std::make_shared<RotatingComponent>(pacman2.get(), 40.f, 4.f));
	scene.Add(pacman2);
	pacman2->SetParent(pacman1.get());


}

int main(int, char*[]) {
	ArcaneEngine engine("../Data/");
	engine.Run(load);
    return 0;
}
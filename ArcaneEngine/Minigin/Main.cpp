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
	logo->SetPosition(216, 180);
	scene.Add(logo);
	
	// The Text
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textObject = std::make_shared<GameObject>();
	textObject->AddComponent(std::make_shared<TextComponent>(textObject.get(), "Programming 4 Assignment", font));
	textObject->SetPosition(80, 280);
	scene.Add(textObject);

	// FPS Counter
	auto fpsObject = std::make_shared<GameObject>();
	fpsObject->AddComponent(std::make_shared<FpsComponent>(fpsObject.get()));
	fpsObject->AddComponent(std::make_shared<TextComponent>(fpsObject.get(), "", font));
	fpsObject->SetPosition(80, 80);
	scene.Add(fpsObject);
}

int main(int, char*[]) {
	ArcaneEngine engine("../Data/");
	engine.Run(load);
    return 0;
}
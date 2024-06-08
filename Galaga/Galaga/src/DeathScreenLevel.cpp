#include "DeathScreenLevel.h"
#include "ArcaneEngine.h"
#include "Commands.h"
#include "GalagaGameInstance.h"
#include "Observers.h"

#include "ImageComponent.h"
#include "TextComponent.h"
#include "ButtonBoxComponent.h"

using namespace AE;

void DeathScreenLevel::Load(AE::Scene& scene)
{
	DrawBackground(scene);
	AddResults(scene);
	AddContinueButton(scene);

	// Mute command
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_M, InputType::IsUpThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AE::ServiceLocator::GetAudio()->ToggleMute(); })));
}

void DeathScreenLevel::DrawBackground(AE::Scene& scene)
{
	auto go = std::make_shared<AE::GameObject>();
	auto imageComp{ std::make_shared<ImageComponent>(go.get(), "Background_Black.png") };
	imageComp->SetDestRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
	go->AddComponent(imageComp);

	scene.Add(go);
}
void DeathScreenLevel::AddResults(AE::Scene& scene)
{
	// Font
	auto font = AE::ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 15);

	// Results text
	auto go1 = std::make_shared<AE::GameObject>();
	go1->AddLocalTransform({220, 120});

	auto text1{ std::make_shared<TextComponent>(go1.get(), "--Results--", font, glm::u8vec3{255, 0, 0}) };
	go1->AddComponent(text1);
	scene.Add(go1);

	// Shots Fired text
	auto go2 = std::make_shared<AE::GameObject>();
	go2->AddLocalTransform({ 180, 160 });

	auto text2{ std::make_shared<TextComponent>(go2.get(), "Shots Fired:", font, glm::u8vec3{255, 255, 0}) };
	go2->AddComponent(text2);
	scene.Add(go2);

	// Shots Hit text
	auto go3 = std::make_shared<AE::GameObject>();
	go3->AddLocalTransform({ 180, 190 });

	auto text3{ std::make_shared<TextComponent>(go3.get(), "Shots Hit:", font, glm::u8vec3{255, 255, 0}) };
	go3->AddComponent(text3);
	scene.Add(go3);

	// Shots Hit text
	auto go4 = std::make_shared<AE::GameObject>();
	go4->AddLocalTransform({ 180, 220 });

	auto text4{ std::make_shared<TextComponent>(go4.get(), "Hit Ratio:", font, glm::u8vec3{255, 255, 255}) };
	go4->AddComponent(text4);
	scene.Add(go4);

	// Score text
	auto go5 = std::make_shared<AE::GameObject>();
	go5->AddLocalTransform({ 180, 280 });

	auto text5{ std::make_shared<TextComponent>(go5.get(), "Score:", font, glm::u8vec3{0, 255, 255}) };
	go5->AddComponent(text5);
	scene.Add(go5);


	// The actual Numbers
	auto baseGI{ SceneManager::GetInstance().GetGameInstance() };
	if (!baseGI) return;
	GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(baseGI)};
	if (!gameInstance) return;

	// Shots fired number
	auto go6 = std::make_shared<AE::GameObject>();
	go6->AddLocalTransform({ 400, 160 });

	std::string shotsFired{ std::to_string(gameInstance->GetShotsFired()) };
	auto text6{ std::make_shared<TextComponent>(go6.get(), shotsFired, font, glm::u8vec3{255, 255, 0}) };
	go6->AddComponent(text6);
	scene.Add(go6);

	// Shots Hit number
	auto go7 = std::make_shared<AE::GameObject>();
	go7->AddLocalTransform({ 400, 190 });

	std::string shotsHit{ std::to_string(gameInstance->GetShotsHit()) };
	auto text7{ std::make_shared<TextComponent>(go7.get(), shotsHit, font, glm::u8vec3{255, 255, 0}) };
	go7->AddComponent(text7);
	scene.Add(go7);

	// Shot Miss Percentage number
	auto go8 = std::make_shared<AE::GameObject>();
	go8->AddLocalTransform({ 400, 220 });


	int shotsFiredInt{ gameInstance->GetShotsFired() };
	int shotsHitInt{ gameInstance->GetShotsHit() };
	std::string hitRatio{ std::to_string(shotsFiredInt == 0 ? 0 : shotsHitInt * 100 / shotsFiredInt ) };

	auto text8{ std::make_shared<TextComponent>(go8.get(), hitRatio + " %", font, glm::u8vec3{255, 255, 255})};
	go8->AddComponent(text8);
	scene.Add(go8);

	// Score number
	auto go9 = std::make_shared<AE::GameObject>();
	go9->AddLocalTransform({ 400, 280 });

	std::string score{ std::to_string(gameInstance->GetScore()) };
	auto text9{ std::make_shared<TextComponent>(go9.get(), score, font, glm::u8vec3{0, 255, 255}) };
	go9->AddComponent(text9);
	scene.Add(go9);
}
void DeathScreenLevel::AddContinueButton(AE::Scene& scene)
{
	// Font
	auto font = AE::ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 18);

	// Button Box
	auto buttonBox = std::make_shared<AE::GameObject>();

	auto imageComp{ std::make_shared<ImageComponent>(buttonBox.get(), "SelectionArrow.png") };
	imageComp->SetDestRect({ 0, 0, 16, 16 });
	buttonBox->AddComponent(imageComp);

	auto buttonBoxComp{ std::make_shared<ButtonBoxComponent>(buttonBox.get(), imageComp.get(), glm::vec2{-30.f, 3.f}) };
	buttonBox->AddComponent(buttonBoxComp);

	buttonBox->AddObserver(std::move(std::make_unique<ButtonBoxObserver>()));

	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_A, AE::InputType::IsUpThisFrame, std::move(std::make_unique<ButtonPressCommand>(buttonBox.get())));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_SPACE, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonPressCommand>(buttonBox.get())));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_RETURN, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonPressCommand>(buttonBox.get())));
	
	scene.Add(buttonBox);

	// Button
	auto button = std::make_shared<AE::GameObject>();
	button->AddLocalTransform({ 245.f, 370.f });

	auto buttonComp{ std::make_shared<ButtonComponent>(button.get(), []()
											{ AE::SceneManager::GetInstance().SetScene("LeaderboardScreen"); }) };
	button->AddComponent(buttonComp);

	auto textComp{ std::make_shared<TextComponent>(button.get(), "Continue", font) };
	button->AddComponent(textComp);

	scene.Add(button);


	// Adding button to ButtonBox
	buttonBoxComp->AddVerticalButton(buttonComp.get());
}

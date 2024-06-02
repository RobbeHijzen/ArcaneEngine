#include "StartScreenLevel.h"

#include "Commands.h"
#include "ImageComponent.h"
#include "ButtonBoxComponent.h"
#include "TextComponent.h"

using namespace AE;

void StartScreenLevel::Load(AE::Scene& scene)
{
	AddBackgroundImage(scene);
	AddLogoImage(scene);
	AddButtons(scene);

}

void StartScreenLevel::AddBackgroundImage(AE::Scene& scene)
{
	auto go = std::make_shared<AE::GameObject>();
	auto imageComp{ std::make_shared<ImageComponent>(go.get(), "Background.png") };
	imageComp->SetDestRect({ 0, 0, 640, 480 });
	
	go->AddComponent(imageComp);
	scene.Add(go);
}

void StartScreenLevel::AddLogoImage(AE::Scene& scene)
{
	auto go = std::make_shared<AE::GameObject>();
	auto imageComp{ std::make_shared<ImageComponent>(go.get(), "GalagaLogo.png") };
	imageComp->SetDestRect({ 180, 20, 280, 120 });

	go->AddComponent(imageComp);
	scene.Add(go);
}

void StartScreenLevel::AddButtons(AE::Scene& scene)
{
	// Creating the ButtonBox itself
	auto buttonBox = std::make_shared<AE::GameObject>();

	auto imageComp{ std::make_shared<ImageComponent>(buttonBox.get(), "SelectionArrow.png") };
	imageComp->SetDestRect({ 0, 0, 16, 16 });
	buttonBox->AddComponent(imageComp);

	auto buttonBoxComp{ std::make_shared<ButtonBoxComponent>(buttonBox.get(), imageComp.get(), glm::vec2{-30.f, 3.f}) };
	buttonBox->AddComponent(buttonBoxComp);

	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_A, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonPressCommand>(buttonBox.get())));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{-1, 0})));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{1, 0})));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_UP, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{0, -1})));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_DOWN, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{0, 1})));
	scene.Add(buttonBox);

	//---------------------------
	// Creating individual Buttons
	//---------------------------

	// Font
	auto font = ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 18);

	// Button 01
	auto button_01 = std::make_shared<AE::GameObject>();
	button_01->AddLocalTransform({ 270.f, 170.f });

	auto buttonComp_01{ std::make_shared<ButtonComponent>(button_01.get(), []() 
											{ AE::SceneManager::GetInstance().SetScene("SoloLevel"); })};
	button_01->AddComponent(buttonComp_01);
	
	auto textComp_01{std::make_shared<TextComponent>(button_01.get(), "Solo", font)};
	button_01->AddComponent(textComp_01);

	scene.Add(button_01);

	// Button 02
	auto button_02 = std::make_shared<AE::GameObject>();
	button_02->AddLocalTransform({ 270.f, 220.f });

	auto buttonComp_02{ std::make_shared<ButtonComponent>(button_02.get(), []() { std::cout << "Button 2 Pressed\n"; }) };
	button_02->AddComponent(buttonComp_02);

	auto textComp_02{ std::make_shared<TextComponent>(button_02.get(), "Coop", font) };
	button_02->AddComponent(textComp_02);

	scene.Add(button_02);

	// Button 03
	auto button_03 = std::make_shared<AE::GameObject>();
	button_03->AddLocalTransform({ 252.f, 270.f });

	auto buttonComp_03{ std::make_shared<ButtonComponent>(button_03.get(), []() { std::cout << "Button 3 Pressed\n"; }) };
	button_03->AddComponent(buttonComp_03);

	auto textComp_03{ std::make_shared<TextComponent>(button_03.get(), "Versus", font) };
	button_03->AddComponent(textComp_03);

	scene.Add(button_03);


	buttonBoxComp->AddVerticalButton(buttonComp_01.get());
	buttonBoxComp->AddVerticalButton(buttonComp_02.get());
	buttonBoxComp->AddVerticalButton(buttonComp_03.get());
}

#include "HighscoreLevel.h"
#include "ResourceManager.h"
#include "ArcaneEngine.h"
#include "GameObject.h"
#include "GalagaGameInstance.h"
#include "Commands.h"

#include "TextComponent.h"
#include "ImageComponent.h"
#include "ButtonBoxComponent.h"

#include <fstream>
#include <algorithm>

using namespace AE;

void HighscoreLevel::Load(AE::Scene& scene)
{
	DrawBackground(scene);
	AddHighscoresText(scene);

	AddScoreInfos(scene);
	AddButtons(scene);
}

void HighscoreLevel::DrawBackground(AE::Scene& scene)
{
	auto go = std::make_shared<AE::GameObject>();
	auto imageComp{ std::make_shared<ImageComponent>(go.get(), "Background_Black.png") };
	imageComp->SetDestRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
	go->AddComponent(imageComp);

	scene.Add(go);
}

void HighscoreLevel::AddHighscoresText(AE::Scene& scene)
{
	// Font
	auto font = AE::ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 15);

	// Results text
	auto go1 = std::make_shared<AE::GameObject>();
	go1->AddLocalTransform({ 180, 40 });

	auto text1{ std::make_shared<TextComponent>(go1.get(), "----HighScores----", font, glm::u8vec3{255, 0, 0}) };
	go1->AddComponent(text1);
	scene.Add(go1);
}
void HighscoreLevel::AddScoreInfos(AE::Scene& scene)
{
	std::vector<ScoreInfo> highscores{}; // the bool indicated whether this is the player's score

	if (std::ifstream input{ "Highscores.txt", std::ios::binary }; input.is_open())
	{
		while (!input.eof())
		{
			ScoreInfo highScore{};

			int stringSize{};
			input.read((char*)&stringSize, sizeof(int));

			char* name{ new char[stringSize] };
			input.read(name, stringSize);
			highScore.name = name;
			delete[] name;

			input.read((char*)&highScore.score, sizeof(int));

			highscores.emplace_back(highScore);
		}
		highscores.erase(highscores.end() - 1);
	}

	// Add current score
	GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
	if (!gameInstance) return;

	// Sort highscores
	std::sort(highscores.begin(), highscores.end(), [](const ScoreInfo& a, const ScoreInfo& b) { return a.score > b.score; });

	// Font
	auto fontNumbers = AE::ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 15);
	auto fontNames = AE::ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 12);

	for (int index{ 1 }; index <= 5; ++index)
	{
		// Number
		auto number = std::make_shared<AE::GameObject>();
		number->AddLocalTransform({ 180.f, 60.f + 30 * index });

		auto numberText{ std::make_shared<TextComponent>(number.get(), std::to_string(index) + ".", fontNumbers, glm::u8vec3{ 255, 255, 255 }) };
		number->AddComponent(numberText);
		scene.Add(number);

		if (index - 1 < highscores.size())
		{
			ScoreInfo highscore{ highscores[index - 1] };

			// Name
			auto name = std::make_shared<AE::GameObject>();
			name->AddLocalTransform({ 220.f, 62.f + 30 * index });

			std::string drawString{ highscore.name };
			if (drawString.size() > 13)
			{
				drawString.resize(12);
				drawString.append("..");
			}

			auto nameText{ std::make_shared<TextComponent>(name.get(), drawString, fontNames, glm::u8vec3{ 0, 255, 0 }) };
			name->AddComponent(nameText);
			scene.Add(name);

			// Score
			auto score = std::make_shared<AE::GameObject>();
			score->AddLocalTransform({ 400.f, 60.f + 30 * index });

			auto scoreText{ std::make_shared<TextComponent>(score.get(), std::to_string(highscore.score), fontNumbers, glm::u8vec3{ 0, 255, 255 }) };
			score->AddComponent(scoreText);
			scene.Add(score);
		}
	}
}

void HighscoreLevel::AddButtons(AE::Scene& scene)
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

	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_A, AE::InputType::IsUpThisFrame, std::move(std::make_unique<ButtonPressCommand>(buttonBox.get())));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ -1, 0 })));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 1, 0 })));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_UP, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 0, -1 })));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_DOWN, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 0, 1 })));

	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_SPACE, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonPressCommand>(buttonBox.get())));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_A, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ -1, 0 })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_D, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 1, 0 })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_W, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 0, -1 })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_S, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 0, 1 })));

	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_RETURN, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonPressCommand>(buttonBox.get())));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_LEFT, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ -1, 0 })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_RIGHT, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 1, 0 })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_UP, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 0, -1 })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_DOWN, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 0, 1 })));

	scene.Add(buttonBox);

	// Return to MainMenu Button
	auto backButton = std::make_shared<AE::GameObject>();
	backButton->AddLocalTransform({ 270.f, 330.f });

	auto backButtonComp{ std::make_shared<ButtonComponent>(backButton.get(), []()
											{ AE::SceneManager::GetInstance().SetScene("StartScreen"); }) };
	backButton->AddComponent(backButtonComp);

	auto backTextComp{ std::make_shared<TextComponent>(backButton.get(), "Back", font) };
	backButton->AddComponent(backTextComp);

	scene.Add(backButton);

	// Adding button to ButtonBox
	buttonBoxComp->AddVerticalButton(backButtonComp.get());
}

#include "LeaderboardLevel.h"
#include "ResourceManager.h"
#include "ArcaneEngine.h"
#include "GameObject.h"
#include "GalagaGameInstance.h"
#include "Commands.h"
#include "Observers.h"

#include "TextComponent.h"
#include "ImageComponent.h"
#include "ButtonBoxComponent.h"

#include <fstream>
#include <algorithm>

using namespace AE;

void LeaderboardLevel::Load(AE::Scene& scene)
{
	DrawBackground(scene);
	AddHighscoresText(scene);

	AddScoreInfos(scene);
	AddButtons(scene);

	// Mute command
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_M, InputType::IsUpThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AE::ServiceLocator::GetAudio()->ToggleMute(); })));
}

void LeaderboardLevel::DrawBackground(AE::Scene& scene)
{
	auto go = std::make_shared<AE::GameObject>();
	auto imageComp{ std::make_shared<ImageComponent>(go.get(), "Background_Black.png") };
	imageComp->SetDestRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
	go->AddComponent(imageComp);

	scene.Add(go);
}

void LeaderboardLevel::AddHighscoresText(AE::Scene& scene)
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

void LeaderboardLevel::AddScoreInfos(AE::Scene& scene)
{
	std::vector<std::pair<ScoreInfo, bool>> highscores{}; // the bool indicated whether this is the player's score

    if (std::ifstream input{ AE::ResourceManager::GetInstance().GetFullPathFromPath("Highscores.txt"), std::ios::binary }; input.is_open())
    {
        while (!input.eof())
        {
			ScoreInfo highScore{};

			int stringSize{};
			input.read((char*)&stringSize, sizeof(int));

			char* name{ new char[stringSize]};
			input.read(name, stringSize);
			highScore.name = name;
			delete[] name;

			input.read((char*)&highScore.score, sizeof(int));

			highscores.emplace_back(highScore, false);
        }
		highscores.erase(highscores.end() - 1);
    }

	// Add current score
	GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
	if (!gameInstance) return;

	highscores.emplace_back(ScoreInfo{ "You", gameInstance->GetScore() }, true);

	// Sort highscores
	std::sort(highscores.begin(), highscores.end(), [](const std::pair<ScoreInfo, bool>& a, const std::pair<ScoreInfo, bool>& b) { return a.first.score > b.first.score; });

	// Font
	auto fontNumbers = AE::ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 15);
	auto fontNames = AE::ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 12);
	
	int idx{};
	for (const auto& highscore : highscores)
	{
		++idx;

		// Number
		auto number = std::make_shared<AE::GameObject>();
		number->AddLocalTransform({ 180.f, 60.f + 30 * idx });

		glm::u8vec3 colorNumber = highscore.second ? glm::u8vec3{ 255, 255, 0 } : glm::u8vec3{ 255, 255, 255 };
		auto numberText{ std::make_shared<TextComponent>(number.get(), std::to_string(idx) + ".", fontNumbers, colorNumber)};
		number->AddComponent(numberText);
		scene.Add(number);

		// Name
		auto name = std::make_shared<AE::GameObject>();
		name->AddLocalTransform({ 220.f, 62.f + 30 * idx });

		std::string drawString{ highscore.first.name };
		if (drawString.size() > 13)
		{
			drawString.resize(12);
			drawString.append("..");
		}

		glm::u8vec3 colorName = highscore.second ? glm::u8vec3{ 255, 255, 0 } : glm::u8vec3{ 0, 255, 0 };
		auto nameText{ std::make_shared<TextComponent>(name.get(), drawString, fontNames, colorName) };
		name->AddComponent(nameText);
		scene.Add(name);

		// Score
		auto score = std::make_shared<AE::GameObject>();
		score->AddLocalTransform({ 400.f, 60.f + 30 * idx });

		glm::u8vec3 colorScore = highscore.second ? glm::u8vec3{ 255, 255, 0 } : glm::u8vec3{ 0, 255, 255 };
		auto scoreText{ std::make_shared<TextComponent>(score.get(), std::to_string(highscore.first.score), fontNumbers, colorScore) };
		score->AddComponent(scoreText);
		scene.Add(score);

	}
}

void LeaderboardLevel::AddButtons(AE::Scene& scene)
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
	auto saveButton = std::make_shared<AE::GameObject>();
	saveButton->AddLocalTransform({ 230.f, 330.f });

	auto saveButtonComp{ std::make_shared<ButtonComponent>(saveButton.get(), []()
											{ AE::SceneManager::GetInstance().SetScene("SaveScreen"); }) };
	saveButton->AddComponent(saveButtonComp);

	auto saveTextComp{ std::make_shared<TextComponent>(saveButton.get(), "Save Score", font) };
	saveButton->AddComponent(saveTextComp);

	scene.Add(saveButton);

	// Return to MainMenu Button
	auto menuButton = std::make_shared<AE::GameObject>();
	menuButton->AddLocalTransform({ 200.f, 370.f });

	auto menuButtonComp{ std::make_shared<ButtonComponent>(menuButton.get(), []()
											{ 
												GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
												gameInstance->ResetStats();

												AE::SceneManager::GetInstance().SetScene("StartScreen"); 
											}) };
	menuButton->AddComponent(menuButtonComp);

	auto menuTextComp{ std::make_shared<TextComponent>(menuButton.get(), "Return to Menu", font) };
	menuButton->AddComponent(menuTextComp);

	scene.Add(menuButton);


	// Adding button to ButtonBox
	buttonBoxComp->AddVerticalButton(saveButtonComp.get());
	buttonBoxComp->AddVerticalButton(menuButtonComp.get());
}

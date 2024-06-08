#include "SaveLevel.h"
#include "ResourceManager.h"
#include "ArcaneEngine.h"
#include "GameObject.h"
#include "Commands.h"
#include "Observers.h"

#include "ImageComponent.h"
#include "ButtonBoxComponent.h"

#include <fstream>
#include <algorithm>

using namespace AE;

void SaveLevel::Load(AE::Scene& scene)
{
	// Greyed out background
	DrawBackground(scene);
	AddHighscoresText(scene);

	std::vector<ScoreInfo> highscores{};
	AddScoreInfos(scene, highscores);

	auto go = std::make_shared<AE::GameObject>();
	auto imageComp{ std::make_shared<ImageComponent>(go.get(), "GradientOverlay.png") };
	imageComp->SetDestRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
	go->AddComponent(imageComp);

	scene.Add(go);

	// keyboard and name stuff
	AddCentralNameText(scene);
	AddButtons(scene, highscores);
}

void SaveLevel::DrawBackground(AE::Scene& scene)
{
	auto go = std::make_shared<AE::GameObject>();
	auto imageComp{ std::make_shared<ImageComponent>(go.get(), "Background_Black.png") };
	imageComp->SetDestRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
	go->AddComponent(imageComp);

	scene.Add(go);
}
void SaveLevel::AddHighscoresText(AE::Scene& scene)
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
void SaveLevel::AddScoreInfos(AE::Scene& scene, std::vector<ScoreInfo>& finalScores)
{
	std::vector<std::pair<ScoreInfo, bool>> highscores{}; // the bool indicated whether this is the player's score

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

			highscores.emplace_back(highScore, false);
			finalScores.emplace_back(highScore);
		}
		highscores.erase(highscores.end() - 1);
		finalScores.erase(finalScores.end() - 1);
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
		auto numberText{ std::make_shared<TextComponent>(number.get(), std::to_string(idx) + ".", fontNumbers, colorNumber) };
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

void SaveLevel::AddCentralNameText(AE::Scene& scene)
{
	auto font = ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 16);

	auto name = std::make_shared<AE::GameObject>();
	auto nameTextComp{ std::make_shared<TextComponent>(name.get(), " ", font) };
	name->AddComponent(nameTextComp);
	name->SetLocalTransform({ 300, 220 });

	scene.Add(name);
	m_PlayerTextComp = nameTextComp.get();
}
void SaveLevel::AddButtons(AE::Scene& scene, const std::vector<ScoreInfo>& highscores)
{
	// Button Box
	auto buttonBox = std::make_shared<AE::GameObject>();

	auto imageComp{ std::make_shared<ImageComponent>(buttonBox.get(), "outlinedBox.png") };
	imageComp->SetDestRect({ 0, 0, 34, 34 });
	buttonBox->AddComponent(imageComp);

	auto buttonBoxComp{ std::make_shared<ButtonBoxComponent>(buttonBox.get(), imageComp.get(), glm::vec2{-1.f, -1.f}) };
	buttonBox->AddComponent(buttonBoxComp);

	InputManager::GetInstance().RemoveAllBindings();

	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_A, AE::InputType::IsUpThisFrame, std::move(std::make_unique<ButtonPressCommand>(buttonBox.get())));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ -1, 0 })));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 1, 0 })));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_UP, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 0, -1 })));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_DOWN, InputType::IsUpThisFrame, std::move(std::make_unique<ButtonMoveSelectionCommand>(buttonBox.get(), glm::i32vec2{ 0, 1 })));
	
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_Q, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("q"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_W, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("w"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_E, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("e"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_R, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("r"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_T, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("t"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_Y, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("y"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_U, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("u"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_I, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("i"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_O, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("o"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_P, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("p"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_A, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("a"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_S, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("s"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_D, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("d"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_F, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("f"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_G, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("g"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_H, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("h"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_J, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("j"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_K, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("k"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_L, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("l"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_Z, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("z"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_X, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("x"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_C, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("c"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_V, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("v"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_B, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("b"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_N, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("n"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_M, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName("m"); })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_SPACE, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {AppendName(" "); })));
	
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_BACKSPACE, InputType::IsDownThisFrame, std::move(std::make_unique<LambdaCommand>([&]() {if (!m_PlayerName.empty())
					{
						m_PlayerName.pop_back();
						m_PlayerTextComp->SetText(m_PlayerName);
					}  })));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_RETURN, InputType::IsUpThisFrame, std::move(std::make_unique<LambdaCommand>([&, highscores]() {if (m_PlayerName.empty()) return;
					WriteHighscores(highscores, m_PlayerName);
					m_PlayerName = "";

					GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
					gameInstance->ResetStats();

					AE::SceneManager::GetInstance().SetScene("StartScreen");  })));
	
	scene.Add(buttonBox);


	// Special Buttons

	// Space
	auto spaceButton = std::make_shared<AE::GameObject>();
	spaceButton->AddLocalTransform({ 200, 400 });

	auto spaceButtonComp{ std::make_shared<ButtonComponent>(spaceButton.get(), [&]()
											{ AppendName(" "); }) };
	spaceButton->AddComponent(spaceButtonComp);

	auto spaceImageComp{ std::make_shared<ImageComponent>(spaceButton.get(), "keyboard.png") };
	spaceImageComp->SetSourceRect({ 226, 251, 359, 43 });
	spaceImageComp->SetDestRect({ 0, 0, 268, 32 });
	spaceButton->AddComponent(spaceImageComp);

	scene.Add(spaceButton);

	// Backspace
	auto backButton = std::make_shared<AE::GameObject>();
	backButton->AddLocalTransform({ 110 + 40 * 10, 280 });

	auto backButtonComp{ std::make_shared<ButtonComponent>(backButton.get(), [&]()
											{ 
												if (!m_PlayerName.empty()) 
												{ 
													m_PlayerName.pop_back(); 
													m_PlayerTextComp->SetText(m_PlayerName);
												} 
											}) };
	backButton->AddComponent(backButtonComp);

	auto backImageComp{ std::make_shared<ImageComponent>(backButton.get(), "keyboard.png") };
	backImageComp->SetSourceRect({ 661, 67, 75, 42 });
	backImageComp->SetDestRect({ 0, 0, 56, 32 });
	backButton->AddComponent(backImageComp);

	scene.Add(backButton);

	// Enter
	auto enterButton = std::make_shared<AE::GameObject>();
	enterButton->AddLocalTransform({ 128 + 40 * 9, 320 });

	auto enterButtonComp{ std::make_shared<ButtonComponent>(enterButton.get(), [&, highscores]()
					{ 
						if (m_PlayerName.empty()) return;
						WriteHighscores(highscores, m_PlayerName); 
						m_PlayerName = "";

						GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
						gameInstance->ResetStats();

						AE::SceneManager::GetInstance().SetScene("StartScreen"); 
					}) };
	enterButton->AddComponent(enterButtonComp);

	auto enterImageComp{ std::make_shared<ImageComponent>(enterButton.get(), "keyboard.png") };
	enterImageComp->SetSourceRect({ 663, 162, 71, 42 });
	enterImageComp->SetDestRect({ 0, 0, 54, 32 });
	enterButton->AddComponent(enterImageComp);

	scene.Add(enterButton);

	// Buttons
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 0, 280}, "q", {109, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 1, 280}, "w", {158, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 2, 280}, "e", {206, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 3, 280}, "r", {254, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 4, 280}, "t", {303, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 5, 280}, "y", {352, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 6, 280}, "u", {400, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 7, 280}, "i", {449, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 8, 280}, "o", {497, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, {110 + 40 * 9, 280}, "p", {546, 113, 43, 43}, "keyboard.png"), {34, 34});
	buttonBoxComp->AddHorizontalButton(backButtonComp.get(), { 58, 34 });

	buttonBoxComp->AddVerticalButton(  MakeButton(scene, { 128 + 40 * 0, 320 }, "a", { 129, 160, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 128 + 40 * 1, 320 }, "s", { 178, 160, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 128 + 40 * 2, 320 }, "d", { 227, 160, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 128 + 40 * 3, 320 }, "f", { 276, 160, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 128 + 40 * 4, 320 }, "g", { 324, 160, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 128 + 40 * 5, 320 }, "h", { 373, 160, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 128 + 40 * 6, 320 }, "j", { 422, 160, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 128 + 40 * 7, 320 }, "k", { 471, 160, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 128 + 40 * 8, 320 }, "l", { 520, 160, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(enterButtonComp.get(), {56, 34});

	buttonBoxComp->AddVerticalButton(  MakeButton(scene, { 142 + 40 * 0, 360 }, "z", { 144, 206, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 142 + 40 * 1, 360 }, "x", { 192, 206, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 142 + 40 * 2, 360 }, "c", { 241, 206, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 142 + 40 * 3, 360 }, "v", { 290, 206, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 142 + 40 * 4, 360 }, "b", { 338, 206, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 142 + 40 * 5, 360 }, "n", { 387, 206, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddHorizontalButton(MakeButton(scene, { 142 + 40 * 6, 360 }, "m", { 435, 206, 43, 43 }, "keyboard.png"), { 34, 34 });
	buttonBoxComp->AddVerticalButton(spaceButtonComp.get(), { 270, 34 });
}
ButtonComponent* SaveLevel::MakeButton(AE::Scene& scene, glm::vec2 pos, std::string addition, AE::Rect sourceRect, std::string texture)
{
	auto button = std::make_shared<AE::GameObject>();
	button->AddLocalTransform({ pos.x, pos.y });

	auto buttonComp{ std::make_shared<ButtonComponent>(button.get(), [&, addition]()
											{ AppendName(addition); }) };
	button->AddComponent(buttonComp);

	auto imageComp{ std::make_shared<ImageComponent>(button.get(), texture) };
	imageComp->SetSourceRect(sourceRect);
	imageComp->SetDestRect({ 0, 0, 32, 32 });
	button->AddComponent(imageComp);

	scene.Add(button);

	return buttonComp.get();
}

void SaveLevel::WriteHighscores(const std::vector<ScoreInfo>& originalScores, std::string newName)
{
	std::vector<ScoreInfo> highscores{ originalScores };

	if (std::ofstream output{ "Highscores.txt", std::ios::binary }; output.is_open())
	{
		GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
		if (!gameInstance) return;

		highscores.emplace_back(ScoreInfo{ newName, gameInstance->GetScore() });
		std::sort(highscores.begin(), highscores.end(), [](const ScoreInfo& a, const ScoreInfo& b) { return a.score > b.score; });

		if (highscores.size() > 5)
		{
			highscores.erase(highscores.end() - 1);
		}

		for (const auto& highscore : highscores)
		{
			int nameSize{ static_cast<int>(highscore.name.size() + 1) };
			output.write((const char*)&nameSize, sizeof(int));

			output.write(highscore.name.c_str(), highscore.name.size() + 1);
			output.write((const char*)&highscore.score, sizeof(int));
		}
	}
}

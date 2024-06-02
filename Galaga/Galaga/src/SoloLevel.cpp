#include "SoloLevel.h"

#include "TextComponent.h"
#include "ImageComponent.h"
#include "ScrollingImageComponent.h"
#include "HealthComponent.h"
#include "ShootComponent.h"

#include "Observers.h"
#include "Commands.h"

#include "FSMComponent.h"
#include "StatesEnemyBoss.h"

using namespace AE;

void SoloLevel::Load(Scene& scene)
{
	AddBackgroundImage(scene);
	auto galaga{ AddGalaga(scene) };
	AddBossEnemy(scene, galaga);

	auto go = std::make_shared<AE::GameObject>();
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_RETURN, InputType::IsUpThisFrame, std::move(std::make_unique<LoadCommand>(go.get(), "DeathScreen")));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_Y, InputType::IsUpThisFrame, std::move(std::make_unique<LoadCommand>(go.get(), "DeathScreen")));

	scene.Add(go);
}

void SoloLevel::AddBackgroundImage(Scene& scene)
{
	auto backgroundImage = std::make_shared<AE::GameObject>();
	auto scrollingComp{ std::make_shared<ScrollingImageComponent>(backgroundImage.get(), "Background.png", 50.f) };
	scrollingComp->SetDestRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
	backgroundImage->AddComponent(scrollingComp);
	scene.Add(backgroundImage);
}

AE::GameObject* SoloLevel::AddGalaga(Scene& scene)
{
	auto galaga = std::make_shared<AE::GameObject>();
	auto imageComp{ std::make_shared<ImageComponent>(galaga.get(), "Galaga.png") };
	imageComp->SetDestRect({0, 0, 50, 50 });
	imageComp->SetSourceRect({ 109, 1, 16, 16 });
	galaga->AddComponent(imageComp);
	galaga->AddComponent(std::make_shared<HealthComponent>(galaga.get(), 4));

	// Shoot component
	auto shootComp{ std::make_shared<ShootComponent>(galaga.get())};
	shootComp->SetBulletSpawnOffset({ 7.5f, -17.f });
	shootComp->AddIgnoreTag("Friendly");

	galaga->AddComponent(shootComp);
	
	// Hitbox
	galaga->AddComponent(std::make_shared<HitboxComponent>(galaga.get(), 50.f, 50.f));

	galaga->SetLocalTransform({ WINDOW_WIDTH / 2.f - 25.f, WINDOW_HEIGHT - 100.f });
	galaga->AddTag("Friendly");

	constexpr float galagaMoveSpeed{ 130.f };
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_A, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ -1.f, 0.f }, galagaMoveSpeed)));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_D, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ 1.f, 0.f }, galagaMoveSpeed)));

	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ -1.f, 0.f }, galagaMoveSpeed)));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ 1.f, 0.f }, galagaMoveSpeed)));

	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_SPACE, InputType::IsDownThisFrame, std::move(std::make_unique<ShootCommand>(galaga.get())));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_A, InputType::IsDownThisFrame, std::move(std::make_unique<ShootCommand>(galaga.get())));

	scene.Add(galaga);


	auto font = ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 19);

	auto score = std::make_shared<AE::GameObject>();
	auto scoreTextComp{ std::make_shared<TextComponent>(score.get(), "0", font) };
	score->AddComponent(scoreTextComp);
	score->SetLocalTransform({ 10, 10 });
	scene.Add(score);

	galaga->AddObserver(std::move(std::make_unique<HealthDisplayObserver>("Galaga.png", AE::Rect{ 109, 1, 16, 16 }, AE::Rect{ 20, 440, 35, 35 })));
	AE::SceneManager::GetInstance().GetGameInstance()->AddObserver(std::move(std::make_unique<ScoreDisplayObserver>(scoreTextComp)));
	galaga->AddObserver(std::move(std::make_unique<GalagaObserver>()));

	return galaga.get();
}

void SoloLevel::AddBossEnemy(AE::Scene& scene, AE::GameObject* galaga)
{
	auto enemy = std::make_shared<AE::GameObject>();
	enemy->SetLocalTransform({ WINDOW_WIDTH / 2.f - 20.f, 50.f });
	enemy->AddTag("Enemy");

	// Image Component
	auto imageComp{ std::make_shared<ImageComponent>(enemy.get(), "Galaga.png") };
	imageComp->SetDestRect({0, 0, 40, 40 });
	imageComp->SetSourceRect({ 1, 91, 16, 16 });
	enemy->AddComponent(imageComp);

	// Shoot Component
	auto shootComp{ std::make_shared<ShootComponent>(enemy.get())};
	shootComp->SetBulletDirection(glm::vec2{ 0.f, 1.f });
	shootComp->SetBulletSpeed(200.f);
	shootComp->SetSeekTarget(galaga);
	shootComp->AddIgnoreTag("Enemy");
	shootComp->SetBulletSpawnOffset(glm::vec2{2.5f, 30.f});

	enemy->AddComponent(shootComp);

	// Other Components
	enemy->AddComponent(std::make_shared<HealthComponent>(enemy.get(), 2));
	enemy->AddComponent(std::make_shared<HitboxComponent>(enemy.get(), 40.f, 40.f));

	// Observers
	enemy->AddObserver(std::move(std::make_unique<EnemyObserver>(galaga, 150)));

	// FSM
	auto idleState{ std::make_unique<StatesEnemyBoss::Idle>() };
	enemy->AddComponent(std::make_shared<FSMComponent>(enemy.get(), std::move(idleState)));

	auto fullHealthState{std::make_unique<StatesEnemyBoss::FullHealth>()};
	enemy->AddComponent(std::make_shared<FSMComponent>(enemy.get(), std::move(fullHealthState)));

	
	scene.Add(enemy);
}
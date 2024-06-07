#include "CoopLevel.h"

#include "TextComponent.h"
#include "ImageComponent.h"
#include "ScrollingImageComponent.h"
#include "HealthComponent.h"
#include "ShootComponent.h"
#include "SpawnerManagerComponent.h"
#include "NoOffScreenComponent.h"

#include "Observers.h"
#include "Commands.h"

#include "FSMComponent.h"
#include "StatesEnemyBoss.h"
#include "StatesEnemy.h"

using namespace AE;

void CoopLevel::Load(Scene& scene)
{
	AddBackgroundImage(scene);

	std::vector<AE::GameObject*> galagas{};
	galagas.emplace_back(AddGalaga1(scene));
	galagas.emplace_back(AddGalaga2(scene));

	AddSpawnerManager(scene, galagas);
	CreateSurroundingHitBoxes(scene);
}

void CoopLevel::AddBackgroundImage(Scene& scene)
{
	auto backgroundImage = std::make_shared<AE::GameObject>();
	auto scrollingComp{ std::make_shared<ScrollingImageComponent>(backgroundImage.get(), "Background.png", 50.f) };
	scrollingComp->SetDestRect({ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT });
	backgroundImage->AddComponent(scrollingComp);
	scene.Add(backgroundImage);
}

AE::GameObject* CoopLevel::AddGalaga1(Scene& scene)
{
	auto galaga = std::make_shared<AE::GameObject>();

	galaga->SetLocalTransform({ WINDOW_WIDTH / 2.f - 45.f, WINDOW_HEIGHT - 85.f });
	galaga->AddTag("Friendly");

	// Image Comp
	auto imageComp{ std::make_shared<ImageComponent>(galaga.get(), "Galaga.png") };
	imageComp->SetDestRect({ 0, 0, 30, 30 });
	imageComp->SetSourceRect({ 109, 1, 16, 16 });
	galaga->AddComponent(imageComp);
	galaga->AddComponent(std::make_shared<HealthComponent>(galaga.get(), 4));

	// Shoot component
	auto shootComp{ std::make_shared<ShootComponent>(galaga.get()) };
	shootComp->SetBulletSpawnOffset({ 3.f, -17.f });
	shootComp->SetBulletSpeed(125.f);
	shootComp->AddIgnoreTag("Friendly");
	shootComp->AddIgnoreTag("Bullet");

	galaga->AddComponent(shootComp);

	// Hitbox + NoOffScreen + MoveComp
	auto hitboxComp{ std::make_shared<HitboxComponent>(galaga.get(), 30.f, 30.f) };
	galaga->AddComponent(hitboxComp);
	auto moveComp{ std::make_shared<MovementComponent>(galaga.get()) };
	galaga->AddComponent(moveComp);
	galaga->AddComponent(std::make_shared<NoOffScreenComponent>(galaga.get(), hitboxComp.get()));

	constexpr float galagaMoveSpeed{ 70.f };
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_A, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ -1.f, 0.f }, galagaMoveSpeed, moveComp.get())));
	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_D, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ 1.f, 0.f }, galagaMoveSpeed, moveComp.get())));

	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ -1.f, 0.f }, galagaMoveSpeed, moveComp.get())));
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ 1.f, 0.f }, galagaMoveSpeed, moveComp.get())));

	InputManager::GetInstance().BindActionKB(SDL_SCANCODE_SPACE, InputType::IsDownThisFrame, std::move(std::make_unique<ShootCommand>(galaga.get())));
	InputManager::GetInstance().BindActionGP(1, INPUT_GAMEPAD_A, InputType::IsDownThisFrame, std::move(std::make_unique<ShootCommand>(galaga.get())));

	scene.Add(galaga);


	auto font = ResourceManager::GetInstance().LoadFont("ArcadeFont.ttf", 19);

	auto score = std::make_shared<AE::GameObject>();
	auto scoreTextComp{ std::make_shared<TextComponent>(score.get(), "0", font) };
	score->AddComponent(scoreTextComp);
	score->SetLocalTransform({ 10, 10 });
	scene.Add(score);

	galaga->AddObserver(std::move(std::make_unique<HealthDisplayObserver>("Galaga.png", AE::Rect{ 109, 1, 16, 16 }, AE::Rect{ 20, 440, 35, 35 })));
	galaga->AddObserver(std::move(std::make_unique<ScoreDisplayObserver>(scoreTextComp)));
	galaga->AddObserver(std::move(std::make_unique<GalagaObserver>()));

	return galaga.get();
}
AE::GameObject* CoopLevel::AddGalaga2(Scene& scene)
{
	auto galaga = std::make_shared<AE::GameObject>();

	galaga->SetLocalTransform({ WINDOW_WIDTH / 2.f + 45.f, WINDOW_HEIGHT - 85.f });
	galaga->AddTag("Friendly");

	// Image Comp
	auto imageComp{ std::make_shared<ImageComponent>(galaga.get(), "Galaga.png") };
	imageComp->SetDestRect({ 0, 0, 30, 30 });
	imageComp->SetSourceRect({ 127, 1, 16, 16 });
	galaga->AddComponent(imageComp);
	galaga->AddComponent(std::make_shared<HealthComponent>(galaga.get(), 4));

	// Shoot component
	auto shootComp{ std::make_shared<ShootComponent>(galaga.get()) };
	shootComp->SetBulletSpawnOffset({ 3.f, -17.f });
	shootComp->SetBulletSpeed(125.f);
	shootComp->AddIgnoreTag("Friendly");
	shootComp->AddIgnoreTag("Bullet");

	galaga->AddComponent(shootComp);

	// Hitbox + NoOffScreen + MoveComp
	auto hitboxComp{ std::make_shared<HitboxComponent>(galaga.get(), 30.f, 30.f) };
	galaga->AddComponent(hitboxComp);
	auto moveComp{ std::make_shared<MovementComponent>(galaga.get()) };
	galaga->AddComponent(moveComp);
	galaga->AddComponent(std::make_shared<NoOffScreenComponent>(galaga.get(), hitboxComp.get()));

	constexpr float galagaMoveSpeed{ 70.f };
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_LEFT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ -1.f, 0.f }, galagaMoveSpeed, moveComp.get())));
	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_DPAD_RIGHT, InputType::IsPressed, std::move(std::make_unique<MoveCommand>(galaga.get(), glm::vec2{ 1.f, 0.f }, galagaMoveSpeed, moveComp.get())));

	InputManager::GetInstance().BindActionGP(0, INPUT_GAMEPAD_A, InputType::IsDownThisFrame, std::move(std::make_unique<ShootCommand>(galaga.get())));

	scene.Add(galaga);

	galaga->AddObserver(std::move(std::make_unique<HealthDisplayObserver>("Galaga.png", AE::Rect{ 127, 1, 16, 16 }, AE::Rect{ 519, 440, 35, 35 })));
	galaga->AddObserver(std::move(std::make_unique<GalagaObserver>()));

	return galaga.get();
}

void CoopLevel::AddSpawnerManager(AE::Scene& scene, std::vector<AE::GameObject*> galagas)
{
	auto go = std::make_shared<AE::GameObject>();
	auto spawnerManagerComp{ std::make_shared<SpawnerManagerComponent>(go.get(), galagas) };
	go->AddComponent(spawnerManagerComp);
	scene.Add(go);
}
void CoopLevel::CreateSurroundingHitBoxes(AE::Scene& scene)
{
	auto box1 = std::make_shared<AE::GameObject>();
	auto box2 = std::make_shared<AE::GameObject>();
	auto box3 = std::make_shared<AE::GameObject>();
	auto box4 = std::make_shared<AE::GameObject>();

	auto hitboxComp1{ std::make_shared<HitboxComponent>(box1.get(), float(WINDOW_WIDTH), 1.f) };
	hitboxComp1->SetLocalPosition(0.f, -30.f);
	auto hitboxComp2{ std::make_shared<HitboxComponent>(box2.get(), float(WINDOW_WIDTH), 1.f) };
	hitboxComp2->SetLocalPosition(0.f, WINDOW_HEIGHT + 30.f);
	auto hitboxComp3{ std::make_shared<HitboxComponent>(box3.get(), 1.f, float(WINDOW_HEIGHT)) };
	hitboxComp3->SetLocalPosition(-30.f, 0.f);
	auto hitboxComp4{ std::make_shared<HitboxComponent>(box4.get(), 1.f, float(WINDOW_HEIGHT)) };
	hitboxComp4->SetLocalPosition(WINDOW_WIDTH + 30.f, 0.f);

	box1->AddComponent(hitboxComp1);
	box2->AddComponent(hitboxComp2);
	box3->AddComponent(hitboxComp3);
	box4->AddComponent(hitboxComp4);

	scene.Add(box1);
	scene.Add(box2);
	scene.Add(box3);
	scene.Add(box4);
}

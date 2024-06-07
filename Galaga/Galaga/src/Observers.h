#pragma once
#include "ArcaneEngine.h"

#include "TextComponent.h"
#include "ImageComponent.h"
#include "SpawnerManagerComponent.h"
#include "ShootComponent.h"

class HealthDisplayObserver : public AE::Observer
{
public:

	HealthDisplayObserver(std::string fileName, AE::Rect sourceRect, AE::Rect destRect)
		: m_ImageFileName{ fileName }
		, m_SourceRect{ sourceRect }
		, m_DestRect{ destRect }
	{}

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	std::vector<AE::GameObject*> m_HealthImageGameObjects{};

	std::string m_ImageFileName;
	AE::Rect m_SourceRect;
	AE::Rect m_DestRect;

	void CreateImages(int health);

};

class ScoreDisplayObserver : public AE::Observer
{
public:

	ScoreDisplayObserver(std::shared_ptr<TextComponent> textComp)
		: m_pTextComponent{ textComp }
	{}

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	std::shared_ptr<TextComponent> m_pTextComponent{};

};

class ShootingObserver : public AE::Observer
{
public:
	ShootingObserver();

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	unsigned short m_ShotSoundID{};

};

class BulletObserver : public AE::Observer
{
public:

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

};

class BeamObserver : public AE::Observer
{
public:
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:
	float m_MaxInBeamTime{ 1.f };
	float m_CurrentInBeamTime{};
	bool m_IsInSuckState{false};
};

class EnemyObserver : public AE::Observer
{
public:

	EnemyObserver(AE::GameObject* galaga, int scoreOnDeath, int divingScoreOnDeath);
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	AE::GameObject* m_GalagaObject;
	int m_ScoreOnDeath{};
	int m_DivingScoreOnDeath{};

	

};

class GalagaObserver : public AE::Observer
{
public:

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

};

class SpawnedEnemyObserver : public AE::Observer
{
public:

	SpawnedEnemyObserver(SpawnerManagerComponent* spawnerComp) : m_SpawnerManagerComp{ spawnerComp } {}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	SpawnerManagerComponent* m_SpawnerManagerComp{};
};

class SpawnedBulletObserver : public AE::Observer
{
public:

	SpawnedBulletObserver(ShootComponent* shootComp) : m_ShootComp{ shootComp } {}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	ShootComponent* m_ShootComp{};
};




class StateMachineObserver : public AE::Observer
{
public:

	StateMachineObserver(SpawnerManagerComponent* spawnerComp) : m_SpawnerManagerComp{ spawnerComp } {}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:
	
	SpawnerManagerComponent* m_SpawnerManagerComp{};
};



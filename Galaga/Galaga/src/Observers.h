#pragma once
#include "ArcaneEngine.h"
#include "ServiceLocator.h"
#include "AudioClip.h"

#include "TextComponent.h"
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
	void SpawnExplosionVFX(AE::GameObject* bulletGO);
};

class BeamObserver : public AE::Observer
{
public:
	BeamObserver();
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:
	float m_MaxInBeamTime{ 1.f };
	float m_CurrentInBeamTime{};
	bool m_IsInSuckState{false};

	unsigned short m_BeamSoundID{};
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

	unsigned short m_EnemyDeathSoundID{};

};

class GalagaObserver : public AE::Observer
{
public:
	GalagaObserver(SpawnerManagerComponent* spawnerComp)
		: m_SpawnerComp{spawnerComp}
	{
		m_CapturedShipSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/CapturedShip.mp3", 10);
		m_PlayerDeathSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/PlayerDies.mp3", 10);

	}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:
	unsigned short m_CapturedShipSoundID{};
	unsigned short m_PlayerDeathSoundID{};
	SpawnerManagerComponent* m_SpawnerComp{};
};

class SpawnedEnemyObserver : public AE::Observer
{
public:

	SpawnedEnemyObserver(SpawnerManagerComponent* spawnerComp)
		: m_SpawnerManagerComp{ spawnerComp } 
	{
	}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	SpawnerManagerComponent* m_SpawnerManagerComp{};

};

class EnemyBossObserver : public AE::Observer
{
public:

	EnemyBossObserver()
	{
		m_BossDeathSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/BossDeath.mp3", 10);
	}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	unsigned short m_BossDeathSoundID{};
};

class SpawnedBulletObserver : public AE::Observer
{
public:

	SpawnedBulletObserver(std::shared_ptr<ShootComponent> shootComp) : m_ShootComp{ shootComp } {}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	std::shared_ptr<ShootComponent> m_ShootComp{};
};


class StateMachineObserver : public AE::Observer
{
public:

	StateMachineObserver(SpawnerManagerComponent* spawnerComp) : m_SpawnerManagerComp{ spawnerComp } {}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:
	
	SpawnerManagerComponent* m_SpawnerManagerComp{};
};

class SpawnerManagerObserver : public AE::Observer
{
public:

	SpawnerManagerObserver()
	{
		m_WaveStartSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/Start.mp3", 10, AE::SoundType::Music);
	}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:
	unsigned short m_WaveStartSoundID{};

};

class ButtonBoxObserver : public AE::Observer
{
public:

	ButtonBoxObserver()
	{
		m_ButtonPressSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/ButtonPress.mp3", 10);
		m_ButtonSelectSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/ButtonSelect.mp3", 10);
	}
	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:
	unsigned short m_ButtonPressSoundID{};
	unsigned short m_ButtonSelectSoundID{};

};



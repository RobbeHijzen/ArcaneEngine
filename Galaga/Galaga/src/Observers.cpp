#include "Observers.h"

#include "GameObject.h"
#include "SceneManager.h"

#include "HealthComponent.h"
#include "ShootComponent.h"

#include "GalagaGameInstance.h"

#include "StatesEnemyBee.h"
#include "StatesEnemyButterfly.h"
#include "StatesEnemyBoss.h"
#include "FSMComponent.h"


void HealthDisplayObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::GameStart:
	case AE::Event::ObjectLostHealth:
	{
		int newHealth{ gameObject->GetComponent<HealthComponent>()->GetHealth() };
		CreateImages(newHealth);
	}
	}
}

void HealthDisplayObserver::CreateImages(int health)
{
	health -= 1; // Because the health display should be one less than the actual health
	int size{static_cast<int>( m_HealthImageGameObjects.size() )};

	if (health < size && size > 0)
	{
		for (int index{ size - 1 }; index >= health; --index)
		{
			m_HealthImageGameObjects[index]->Delete();
			m_HealthImageGameObjects.pop_back();
		}
	}
	else if (health > size)
	{
		for (int index{}; index < health; ++index)
		{
			auto go{std::make_shared<AE::GameObject>()};
			auto imageComp{ std::make_shared<ImageComponent>(go.get(), m_ImageFileName) };
			imageComp->SetSourceRect(m_SourceRect);

			auto destRect{ m_DestRect };
			destRect.x += m_DestRect.w * index;
			imageComp->SetDestRect(destRect);
			
			go->AddComponent(imageComp);
			m_HealthImageGameObjects.emplace_back(go.get());
			
			AE::SceneManager::GetInstance().GetCurrentScene()->Add(go);
		}
	}
}

void ScoreDisplayObserver::OnNotify(AE::Event event, AE::GameObject* )
{
	switch (event)
	{
	case AE::Event::ScoreChanged:
	{
		GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
		if (!gameInstance) return;

		std::string newText{ std::to_string(gameInstance->GetScore())};
		m_pTextComponent->SetText(newText);
	}
	}
}

ShootingObserver::ShootingObserver()
{
	m_ShotSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/PlayerShoot.mp3", 10);
}

void ShootingObserver::OnNotify(AE::Event event, AE::GameObject*)
{
	switch (event)
	{
	case AE::Event::FireBullet:
	{
		AE::ServiceLocator::GetAudio()->PlaySound(m_ShotSoundID);

		break;
	}
	}
}

void BulletObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::OnOverlap:
	{
		if (gameObject->IsDeleted()) break;
		if (auto hitbox = gameObject->GetComponent<HitboxComponent>())
		{
			auto overlappedGO{ hitbox->GetLatestOverlapGO()};
			if (auto healthComp = overlappedGO->GetComponent<HealthComponent>())
			{
				healthComp->KillObject();
				SpawnExplosionVFX(gameObject);
			}
		}
		gameObject->Delete();

		break;
	}
	}
	
}

void BulletObserver::SpawnExplosionVFX(AE::GameObject* bulletGO)
{
	constexpr int explosionSize{46};
	constexpr int animationCols{ 5 };
	constexpr float explosionTime{ 0.7f };

	auto go{std::make_shared<AE::GameObject>()};

	glm::vec2 bulletPos{ glm::vec2{bulletGO->GetWorldTransform().GetPosition()} };
	go->SetLocalTransform({ bulletPos.x - explosionSize / 4, bulletPos.y - explosionSize / 2 });

	
	auto animComp{ std::make_shared<AnimationComponent>(go.get(), "Galaga.png", animationCols, explosionTime / animationCols)};
	animComp->SetDestRect({ 0, 0, explosionSize, explosionSize });
	animComp->SetSourceRect({ 289, 1, 32, 32 });
	
	go->AddComponent(animComp);
	
	AE::SceneManager::GetInstance().GetCurrentScene()->Add(go);
	AE::TimeManager::GetInstance().SetTimer([go](int) { go->Delete(); }, explosionTime, 1);
}


BeamObserver::BeamObserver()
{
	m_BeamSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/TractorBeam.mp3", 10);
	AE::ServiceLocator::GetAudio()->PlaySound(m_BeamSoundID);
}

void BeamObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::OnOverlap:
	{
		if (!m_IsInSuckState)
		{
			if (auto hitbox = gameObject->GetComponent<HitboxComponent>())
			{
				auto overlappedGO{ hitbox->GetLatestOverlapGO() };
				if (overlappedGO->HasTag("Friendly"))
				{
					m_CurrentInBeamTime += AE::Time::GetInstance().GetDeltaTime();
					if (m_CurrentInBeamTime >= m_MaxInBeamTime)
					{
						if (auto fsm = gameObject->GetParent()->GetComponent<FSMComponent>())
						{
							if (auto beam = dynamic_cast<StatesEnemyBoss::TractorBeam*>(fsm->GetCurrentState()))
							{
								beam->SwitchToBeamSuck(overlappedGO);
								m_CurrentInBeamTime = 0.f;
								m_IsInSuckState = true;
							}
						}
					}
				}
			}
		}
		break;

		
	}
	}
}

EnemyObserver::EnemyObserver(AE::GameObject* galaga, int scoreOnDeath, int divingScoreOnDeath)
	: m_GalagaObject{ galaga }
	, m_ScoreOnDeath{ scoreOnDeath }
	, m_DivingScoreOnDeath{divingScoreOnDeath}
{
	m_EnemyDeathSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/EnemyDies.mp3", 10);
}

void EnemyObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::ObjectDied:
	{
		AE::ServiceLocator::GetAudio()->PlaySound(m_EnemyDeathSoundID);
		gameObject->Delete();
		if (m_GalagaObject)
		{
			GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
			if (!gameInstance) return;

			if (auto fsm = gameObject->GetComponent<FSMComponent>())
			{
				if(dynamic_cast<StatesEnemyBee::Idle*>(fsm->GetCurrentState()) ||
				   dynamic_cast<StatesEnemyButterfly::Idle*>(fsm->GetCurrentState()) || 
				   dynamic_cast<StatesEnemyBoss::Idle*>(fsm->GetCurrentState()) || 
				   dynamic_cast<StatesEnemyBoss::TractorBeam*>(fsm->GetCurrentState()))
				{
					gameInstance->IncreaseScore(m_ScoreOnDeath, m_GalagaObject);
				}
				else
				{
					gameInstance->IncreaseScore(m_DivingScoreOnDeath, m_GalagaObject);
				}
			}
		}
		
		break;
	}
	case AE::Event::ObjectLostHealth:
	{
		GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
		if (!gameInstance) return;

		gameInstance->IncrementShotsHit();
		break;
	}
	}
}

void GalagaObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::GameStart:
	{
		break;
	}
	case AE::Event::ObjectLostHealth:
	{
		m_SpawnerComp->DeleteAllBullets();
		gameObject->SetLocalTransform(gameObject->GetSpawnTransform());
		AE::ServiceLocator::GetAudio()->PlaySound(m_PlayerDeathSoundID);
		break;
	}
	case AE::Event::ObjectDied:
	{
		AE::SceneManager::GetInstance().SetScene("DeathScreen");
		break;
	}
	case AE::Event::FireBullet:
	{
		GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
		if (!gameInstance) return;

		gameInstance->IncrementShotsFired();
		break;
	}
	case AE::Event::OnOverlap:
	{
		if (auto hitbox = gameObject->GetComponent<HitboxComponent>())
		{
			auto overlappedGO{ hitbox->GetLatestOverlapGO() };
			if (overlappedGO->HasTag("Enemy"))
			{
				bool hitBeamSuck{ false };
				if (auto fsm = overlappedGO->GetComponent<FSMComponent>())
				{
					if (auto beamSuck = dynamic_cast<StatesEnemyBoss::BeamSuck*>(fsm->GetCurrentState()))
					{
						hitBeamSuck = true;
						beamSuck->SpawnRedGalaga(overlappedGO);
						AE::ServiceLocator::GetAudio()->PlaySound(m_CapturedShipSoundID);
					}
				}
				if (!hitBeamSuck)
				{
					if (auto healthCompEnemy = overlappedGO->GetComponent<HealthComponent>())
					{
						healthCompEnemy->KillObject();
					}
				}

				if (auto healthCompPlayer = gameObject->GetComponent<HealthComponent>())
				{
					healthCompPlayer->KillObject();
				}
			}
			
		}
		break;
	}

	}
}

void SpawnedEnemyObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::ObjectDied:
	{
		m_SpawnerManagerComp->RemoveEnemy(gameObject);

		break;
	}
	}
}

void EnemyBossObserver::OnNotify(AE::Event event, AE::GameObject*)
{
	switch (event)
	{
	case AE::Event::ObjectDied:
	{
		AE::ServiceLocator::GetAudio()->PlaySound(m_BossDeathSoundID);

		break;
	}
	}
}


void SpawnedBulletObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::ObjectDestroyed:
	{
		m_ShootComp->RemoveBullet(gameObject);
		break;
	}
	}
}


void StateMachineObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::StateChanged:
	{
		if (auto stateComp = gameObject->GetComponent<FSMComponent>())
		{
			auto currentState{ stateComp->GetCurrentState() };
			if (dynamic_cast<StatesEnemyBee::Idle*>(currentState) ||
				dynamic_cast<StatesEnemyButterfly::Idle*>(currentState) ||
				dynamic_cast<StatesEnemyBoss::Idle*>(currentState))
			{
				m_SpawnerManagerComp->ChangeStateToIdle(gameObject);
			}
			else
			{
				m_SpawnerManagerComp->ChangeStateToNotIdle(gameObject);
			}
		}

		break;
	}
	}
}

void SpawnerManagerObserver::OnNotify(AE::Event event, AE::GameObject*)
{
	switch (event)
	{
	case AE::Event::WaveStarted:
	{
		AE::ServiceLocator::GetAudio()->PlaySound(m_WaveStartSoundID);
		break;
	}
	}
}

void ButtonBoxObserver::OnNotify(AE::Event event, AE::GameObject*)
{
	switch (event)
	{
	case AE::Event::ButtonSelected:
	{
		AE::ServiceLocator::GetAudio()->PlaySound(m_ButtonSelectSoundID);
		break;
	}
	case AE::Event::ButtonPressed:
	{
		AE::ServiceLocator::GetAudio()->PlaySound(m_ButtonPressSoundID);
		break;
	}
	}
}


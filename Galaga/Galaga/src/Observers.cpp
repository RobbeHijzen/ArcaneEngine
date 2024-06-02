#include "Observers.h"

#include "GameObject.h"
#include "SceneManager.h"

#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "ShootComponent.h"

#include "ServiceLocator.h"
#include "GalagaGameInstance.h"


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

void ScoreDisplayObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::ScoreChanged:
	{
		int newScore{ gameObject->GetComponent<ScoreComponent>()->GetScore() };

		std::string newText{ "#Score: " + std::to_string(newScore) };
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
		if (auto hitbox = gameObject->GetComponent<HitboxComponent>())
		{
			auto overlappedGO{ hitbox->GetLatestOverlapGO()};
			if (auto healthComp = overlappedGO->GetComponent<HealthComponent>())
			{
				healthComp->KillObject();
				gameObject->Delete();
			}
		}
		break;
	}
	}
	
}

void BeamObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::OnOverlap:
	{
		if (auto hitbox = gameObject->GetComponent<HitboxComponent>())
		{
			auto overlappedGO{ hitbox->GetLatestOverlapGO() };
			if (auto healthComp = overlappedGO->GetComponent<HealthComponent>())
			{
				healthComp->KillObject();
				hitbox->SetActive(false);
			}
		}
		break;
	}
	}
}

EnemyObserver::EnemyObserver(AE::GameObject* galaga, int scoreOnDeath)
	: m_GalagaObject{galaga}
	, m_ScoreOnDeath{scoreOnDeath}
{
}


void EnemyObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::ObjectDied:
	{
		gameObject->Delete();
		if (m_GalagaObject)
		{
			if (auto scoreComp = m_GalagaObject->GetComponent<ScoreComponent>())
			{
				scoreComp->IncreaseScore(m_ScoreOnDeath);
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
	case AE::Event::ScoreChanged:
	{
		int newScore{ gameObject->GetComponent<ScoreComponent>()->GetScore() };

		GalagaGameInstance* gameInstance{ dynamic_cast<GalagaGameInstance*>(AE::SceneManager::GetInstance().GetGameInstance()) };
		if (!gameInstance) return;

		gameInstance->SetScore(newScore);
	}

	}
}
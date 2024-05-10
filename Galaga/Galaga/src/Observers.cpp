#include "Observers.h"

#include "GameObject.h"

#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "ShootComponent.h"

#include "ServiceLocator.h"


void HealthDisplayObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::GameStart:
	case AE::Event::ObjectLostHealth:
	{
		int newHealth{ gameObject->GetComponent<HealthComponent>()->GetHealth() };

		std::string newText{"#Lives: " + std::to_string(newHealth)};
		m_pTextComponent->SetText(newText);

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
	}
}

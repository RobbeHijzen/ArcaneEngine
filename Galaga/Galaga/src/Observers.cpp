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

void PickupObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::SilverPickup:
	{
		gameObject->GetComponent<ScoreComponent>()->IncreaseScore(SilverScoreValue);
		gameObject->NotifyAll(AE::Event::ScoreChanged);

		break;
	}
	case AE::Event::GoldPickup:
	{
		gameObject->GetComponent<ScoreComponent>()->IncreaseScore(GoldScoreValue);
		gameObject->NotifyAll(AE::Event::ScoreChanged);

		break;
	}
	}
}

ShootingObserver::ShootingObserver()
{
	m_ShotSoundID = AE::ServiceLocator::GetAudio()->CreateSoundClip("Audio/PlayerShoot.mp3", 70);
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

void EnemyObserver::OnNotify(AE::Event event, AE::GameObject* gameObject)
{
	switch (event)
	{
	case AE::Event::ObjectDied:
	{
		gameObject->Delete();
		break;
	}
	}
}

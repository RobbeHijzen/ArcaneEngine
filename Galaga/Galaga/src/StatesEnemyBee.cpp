#include "StatesEnemyBee.h"

#include "StateHelpers.h"
#include "MyTime.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Observers.h"
#include "ShootComponent.h"
#include "StatesEnemy.h"
#include "TimeManager.h"

//****
// Idle
//****
std::unique_ptr<AE::FSMState> StatesEnemyBee::Idle::Update(AE::GameObject* gameObject)
{
	if (m_ChangeToBombingRun)
	{
		auto nextState{ std::make_unique<BombingRun>(m_StartingPos) };

		std::list<EnemySeekInfo> seekInfos{};
		seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Circle, gameObject->GetWorldTransform().GetPosition() });

		return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfos));
	}

	return nullptr;
}

//****
// BombingRun
//****
void StatesEnemyBee::BombingRun::OnEnter(AE::GameObject* gameObject)
{
	m_SeekPos.x = rand() % 2 ? 0.f : WINDOW_WIDTH;
	m_SeekDir = glm::normalize(m_SeekPos - glm::vec2{ gameObject->GetLocalTransform().GetPosition() });

}
std::unique_ptr<AE::FSMState> StatesEnemyBee::BombingRun::Update(AE::GameObject* gameObject)
{
	glm::vec2 addedPosition{ m_SeekDir * m_MoveSpeed * AE::Time::GetInstance().GetDeltaTime() };
	gameObject->AddLocalTransform(AE::Transform{ addedPosition });

	float yHeight{ gameObject->GetWorldTransform().GetPosition().y };
	if (m_CanShoot && yHeight >= m_ShootHeight)
	{
		m_CanShoot = false;

		AE::TimeManager::GetInstance().SetTimer([gameObject](int)
			{
				if (auto shootComp = gameObject->GetComponent<ShootComponent>())
				{
					shootComp->FireBullet();
				}
			}, 0.5f, 2, true);
	}

	if (yHeight > WINDOW_HEIGHT)
	{
		gameObject->AddLocalTransform(AE::Transform{ 0.f, -(WINDOW_HEIGHT + 100.f) });

		auto nextState{ std::make_unique<Idle>(m_IdlePos) };

		std::list<EnemySeekInfo> seekInfos{};
		seekInfos.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, m_IdlePos });
		return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfos));
	}
	return nullptr;
}

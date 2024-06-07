#include "StatesEnemyButterfly.h"

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
std::unique_ptr<AE::FSMState> StatesEnemyButterfly::Idle::Update(AE::GameObject* gameObject)
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
void StatesEnemyButterfly::BombingRun::OnEnter(AE::GameObject* gameObject)
{
	if (gameObject->GetWorldTransform().GetPosition().y < m_MaxShootHeight)
	{
		if (rand() % 100 <= m_BombingChance)
		{
			AE::TimeManager::GetInstance().SetTimer([gameObject](int)
				{
					if (auto shootComp = gameObject->GetComponent<ShootComponent>())
					{
						shootComp->FireBullet();
					}
				}, m_ShootDelay, m_ShootAmount, true);
		}
	}
}
std::unique_ptr<AE::FSMState> StatesEnemyButterfly::BombingRun::Update(AE::GameObject* gameObject)
{
	std::list<EnemySeekInfo> seekInfo{};
	glm::vec2 pos{ gameObject->GetWorldTransform().GetPosition() };

	if (pos.y > WINDOW_HEIGHT)
	{
		gameObject->AddLocalTransform({ 0.f, -(WINDOW_HEIGHT + 100.f) });
		seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, m_IdlePos });

		auto nextState{ std::make_unique<Idle>(m_IdlePos) };
		return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfo));
	}

	glm::vec2 pos_01{ pos.x, pos.y + m_VerticalDistance };
	glm::vec2 pos_02{ pos.x + (rand() % 2 ? m_HorizontalDistance : -m_HorizontalDistance), pos.y + m_VerticalDistance};
	seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, pos_01 });
	seekInfo.emplace_back(EnemySeekInfo{ EnemySeekTypes::Straight, pos_02 });

	auto nextState{ std::make_unique<BombingRun>(m_IdlePos) };
	return std::move(std::make_unique<StatesEnemy::Moving>(std::move(nextState), seekInfo));
}

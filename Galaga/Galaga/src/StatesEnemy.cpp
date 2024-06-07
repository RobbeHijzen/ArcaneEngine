#include "StatesEnemy.h"
#include "MyTime.h"
#include "StateHelpers.h"

constexpr auto F_PI = 3.1415f;

//****
// Spawning
//****
void StatesEnemy::Moving::OnEnter(AE::GameObject* go)
{
	std::list<EnemySeekInfo> newSeekInfo{};

	for (const auto& seekInfo : m_SeekInfo)
	{
		if (seekInfo.seekType == EnemySeekTypes::Circle)
		{
			glm::vec2 dir{ glm::vec2{go->GetWorldTransform().GetPosition()} - seekInfo.seekPos};
			float startingAngle{0.f};

			if (dir.x != 0.f)
			{
				startingAngle = glm::atan(dir.y / dir.x);
				if (dir.x < 0.f) startingAngle += F_PI;
			}
			

			for (int index{}; index < m_CircleSeekAmount; ++index)
			{
				EnemySeekInfo info{};
				info.seekType = EnemySeekTypes::Straight;

				float xPos{ seekInfo.seekPos.x + m_CircleRadius * glm::cos(startingAngle + 2 * F_PI * (float(index) / m_CircleSeekAmount))};
				float yPos{ seekInfo.seekPos.y + m_CircleRadius * glm::sin(startingAngle + 2 * F_PI * (float(index) / m_CircleSeekAmount))};

				info.seekPos = { xPos, yPos };

				newSeekInfo.emplace_back(info);
			}
		}
		else
		{
			newSeekInfo.emplace_back(seekInfo);
		}
	}
	m_SeekInfo = newSeekInfo;
	m_CurrentSeekInfo = m_SeekInfo.front();
}

std::unique_ptr<AE::FSMState> StatesEnemy::Moving::Update(AE::GameObject* gameObject)
{
	MoveAccordingToSeekInfo(m_CurrentSeekInfo, gameObject);

	if (AE::StateHelpers::HasReachedPoint(m_CurrentSeekInfo.seekPos, gameObject))
	{
		if (m_SeekInfo.size() > 1)
		{
			m_SeekInfo.pop_front();
			m_CurrentSeekInfo = m_SeekInfo.front();
		}
		else
		{
			return std::move(m_NextState);
		}
	}

	return nullptr;
}

void StatesEnemy::Moving::MoveAccordingToSeekInfo(EnemySeekInfo seekInfo, AE::GameObject* gameObject)
{
	switch (seekInfo.seekType)
	{
		case EnemySeekTypes::Straight:
		{
			glm::vec2 dir = AE::StateHelpers::GetDirection(seekInfo.seekPos, gameObject);
			glm::vec2 addedPosition{ dir * m_MoveSpeed * AE::Time::GetInstance().GetDeltaTime() };
			gameObject->AddLocalTransform(AE::Transform{ addedPosition });
			break;
		}
		case EnemySeekTypes::Circle:
		{

			break;
		}

		default: break;
	}
}

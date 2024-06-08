#include "AnimationComponent.h"
#include "MyTime.h"

AnimationComponent::AnimationComponent(AE::GameObject* parentGameObject, const std::string& fileName, int colsAmount, float animTime)
	: BaseComponent(parentGameObject)
	, m_AmountCols{colsAmount}
	, m_AnimTime{animTime}
{
	m_ImageComp = std::make_shared<ImageComponent>(parentGameObject, fileName);
	parentGameObject->AddComponent(m_ImageComp);
}

void AnimationComponent::Update()
{
	float dt{AE::Time::GetInstance().GetDeltaTime()};
	m_CurrentTime += dt;
	if (m_CurrentTime >= m_AnimTime)
	{
		m_CurrentTime -= m_AnimTime;
		m_ColIndex = (m_ColIndex + 1) % m_AmountCols;

		glm::vec2 newSourcePos{};
		newSourcePos.x = m_ColIndex * m_OriginalSourceRect.w + m_OriginalSourceRect.x;
		newSourcePos.y = m_OriginalSourceRect.y;

		m_ImageComp->SetSourcePos(newSourcePos);
	}
}

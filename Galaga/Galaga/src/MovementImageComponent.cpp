#include "MovementImageComponent.h"

MovementImageComponent::MovementImageComponent(AE::GameObject* parentGameObject, const std::string& fileName
	, glm::vec2 imageOffset, int numMoveDirections, glm::vec2 startDir)
	: BaseComponent(parentGameObject)
	, m_ImageOffset{imageOffset}
	, m_NumDirections{numMoveDirections}
{
	auto imageComp{ std::make_shared<ImageComponent>(parentGameObject, fileName) };
	m_ImageComp = imageComp.get();
	parentGameObject->AddComponent(imageComp);

	if (startDir.x == 0.f)
	{
		m_StartingDegree = startDir.y < 0.f ? 270.f : 90.f;
	}
	else
	{
		m_StartingDegree = glm::degrees(glm::atan(startDir.y / startDir.x));
		m_StartingDegree = NegativeDegToPosOnlyDeg(m_StartingDegree);
	}
}

void MovementImageComponent::SetSourceRect(AE::Rect sourceRect)
{
	m_OriginalSourceRect = sourceRect;
	m_ImageComp->SetSourceRect(sourceRect);
}

void MovementImageComponent::SetSourcePos(glm::vec2 pos)
{
	m_OriginalSourceRect.x = pos.x;
	m_OriginalSourceRect.y = pos.y;

	m_ImageComp->SetSourcePos(pos);
}
void MovementImageComponent::AddSourcePos(glm::vec2 pos)
{
	m_OriginalSourceRect.x += pos.x;
	m_OriginalSourceRect.y += pos.y;

	m_ImageComp->SetSourcePos({ m_OriginalSourceRect.x, m_OriginalSourceRect.y});
}

void MovementImageComponent::SetDestRect(AE::Rect rect)
{
	m_ImageComp->SetDestRect(rect);
}

void MovementImageComponent::LateUpdate()
{
	glm::vec2 pos{GetOwner()->GetWorldTransform().GetPosition()};
	if (pos == m_LastFramePos)
	{
		m_ImageComp->SetSourceRect(m_OriginalSourceRect);
		m_ImageComp->SetSourcePos({ m_OriginalSourceRect.x, m_OriginalSourceRect.y - 18});

		return;
	}

	glm::vec2 dir{glm::normalize(pos - m_LastFramePos)};
	m_LastFramePos = pos;

	float deg{};
	if (dir.x < 0.f)
	{
		deg = 180.f + glm::degrees(glm::atan(-dir.y / dir.x));
	}
	else
	{
		deg = glm::degrees(glm::atan(-dir.y / dir.x));
	}
	deg = NegativeDegToPosOnlyDeg(NegativeDegToPosOnlyDeg(deg) - m_StartingDegree);
	int index{ m_NumDirections - int((deg - m_DegreeIncrement / 2.f) / m_DegreeIncrement) - 1}; // remove numDirections because the texture is clockwise

	AE::Rect currentSourceRect{m_ImageComp->GetSourceRect()};
	glm::vec2 newSourcePos{};
	newSourcePos.x = m_OriginalSourceRect.x + (currentSourceRect.w + m_ImageOffset.x) * index;
	newSourcePos.y = m_OriginalSourceRect.y;

	m_ImageComp->SetSourcePos(newSourcePos);
}

float MovementImageComponent::NegativeDegToPosOnlyDeg(float degrees)
{
	if (degrees < 0.f)
	{
		return 360 + degrees;
	}
	return degrees;
}

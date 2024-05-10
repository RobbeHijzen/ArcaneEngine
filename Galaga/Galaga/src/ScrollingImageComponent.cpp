#include "ScrollingImageComponent.h"

#include "MyTime.h"
#include "Transform.h"

ScrollingImageComponent::ScrollingImageComponent(AE::GameObject* parentGameObject, const std::string& fileName, float scrollingSpeed)
	: BaseComponent(parentGameObject)
	, m_ScrollingSpeed{ scrollingSpeed }
{
	// if scrollingspeed < 0 then set the scrolling will happen from bottom to top
	m_ScrollSign = scrollingSpeed < 0.f ? 1 : -1;

	m_FirstImage = std::make_shared<ImageComponent>(parentGameObject, fileName);
	parentGameObject->AddComponent(m_FirstImage);
	
	m_SecondImage = std::make_shared<ImageComponent>(parentGameObject, fileName);
	parentGameObject->AddComponent(m_SecondImage);
}

void ScrollingImageComponent::SetSourceRect(AE::Rect sourceRect)
{
	m_FirstImage->SetSourceRect(sourceRect);
	m_SecondImage->SetSourceRect(sourceRect);
}

void ScrollingImageComponent::SetDestRect(AE::Rect destRect)
{
	m_DestHeight = destRect.h;
	m_SecondImage->SetLocalPosition(0.f, m_DestHeight * m_ScrollSign);

	m_FirstImage->SetDestRect(destRect);
	m_SecondImage->SetDestRect(destRect);
}

void ScrollingImageComponent::Update()
{
	float dt{ AE::Time::GetInstance().GetDeltaTime() };

	m_FirstImage->AddLocalPosition(0.f, dt * m_ScrollingSpeed);
	m_SecondImage->AddLocalPosition(0.f, dt * m_ScrollingSpeed);

	if (m_FirstImage->GetLocalTransform().GetPosition().y >= m_DestHeight * m_ScrollSign * -1)
		m_FirstImage->AddLocalPosition(0.f, m_DestHeight * 2 * m_ScrollSign);
	if (m_SecondImage->GetLocalTransform().GetPosition().y >= m_DestHeight * m_ScrollSign * -1)
		m_SecondImage->AddLocalPosition(0.f, m_DestHeight * 2 * m_ScrollSign);
}

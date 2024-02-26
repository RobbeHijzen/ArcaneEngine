#include <stdexcept>
#include <sstream>
#include "FpsComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Time.h"

FpsComponent::FpsComponent(GameObject* const parentGameObject)
	: BaseComponent(parentGameObject)
{ 
}

void FpsComponent::Initialize()
{
	m_pTextComponent = GetOwner()->GetComponent<TextComponent>().get();
}

void FpsComponent::Update()
{
	m_CurrentDelay += Time::GetInstance().GetDeltaTime();
	++m_FrameCount;

	if (m_CurrentDelay >= m_MaxDelay)
	{
		m_pTextComponent->SetText(std::format("{:.1f}{}", m_FrameCount / m_CurrentDelay, " FPS"));

		m_FrameCount = 0;
		m_CurrentDelay = 0;
	}
}

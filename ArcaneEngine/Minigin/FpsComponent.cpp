#include <stdexcept>
#include <sstream>
#include "FpsComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Time.h"

FpsComponent::FpsComponent(GameObject* const parentGameObject)
	: BaseComponent(parentGameObject)
{ 
	m_pTextComponent = GetParent()->GetComponent<TextComponent>().get();
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

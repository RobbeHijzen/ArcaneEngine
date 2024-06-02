#include "ButtonComponent.h"

int ButtonComponent::m_AmountOfButtons{ 0 };

ButtonComponent::ButtonComponent(AE::GameObject* pParent, std::function<void()> pressFunction)
	: BaseComponent(pParent)
	, m_PressFunction{pressFunction}
{
	++m_AmountOfButtons;
	m_ButtonID = m_AmountOfButtons;

}

void ButtonComponent::Press()
{
	m_PressFunction();
}

void ButtonComponent::OnSelect()
{
}

void ButtonComponent::OnDeselect()
{
}

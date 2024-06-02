#pragma once

#include "BaseComponent.h"
#include "GameObject.h"
#include <functional>

class ButtonComponent : public AE::BaseComponent
{
public:
	ButtonComponent(AE::GameObject* pParent, std::function<void()> pressFunction);

	void Press();

	void OnSelect();
	void OnDeselect();

private:
	std::function<void()> m_PressFunction{};

	int m_ButtonID{};
	static int m_AmountOfButtons;
};


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

	void SetOnSelectFunction(std::function<void()> func) { m_OnSelectFunction = func; }
	void SetOnDeselectFunction(std::function<void()> func) { m_OnDeselectFunction = func; }

private:
	std::function<void()> m_PressFunction{};
	std::function<void()> m_OnSelectFunction{ []() {} };
	std::function<void()> m_OnDeselectFunction{ []() {} };

	int m_ButtonID{};
	static int m_AmountOfButtons;
};


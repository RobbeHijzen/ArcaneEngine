#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <WinUser.h>

#include "Controller.h"

class Controller::ControllerImpl
{
public:

	ControllerImpl(int controllerIndex) : m_ControllerIndex{ controllerIndex } {}

	void ProcessInput();
	void BindAction(int key, InputTypeGP inputType, Command* const command)
	{
		m_InputBindings.emplace_back(std::make_unique<InputBindingGP>(key, inputType, command));
	}
private:

	int m_ControllerIndex{ -1 };

	std::vector<std::unique_ptr<InputBindingGP>> m_InputBindings{};
	XINPUT_STATE m_CurrentState{};

	int m_ButtonsPressedThisFrame{};
	int m_ButtonsReleasedThisFrame{};


	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;
};

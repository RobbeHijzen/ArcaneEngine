#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <memory>
#include <type_traits>
#include <WinUser.h>

#include "InputCommands.h"
#include "InputHelperStructs.h"

class Controller
{
public:
	Controller(int controllerIndex);

	bool ProcessInput();

	void BindAction(int key, InputTypeGP inputType, auto command)
	{
		m_InputBindings.emplace_back(std::make_unique<InputBindingGP>(key, inputType, command));
	}

private:

	int m_ControllerIndex{-1};

	std::vector<std::unique_ptr<InputBindingGP>> m_InputBindings{};
	XINPUT_STATE m_CurrentState{};

	int m_ButtonsPressedThisFrame{};
	int m_ButtonsReleasedThisFrame{};


	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;
};


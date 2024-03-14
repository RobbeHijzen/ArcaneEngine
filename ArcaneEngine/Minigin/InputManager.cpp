#include <SDL.h>
#include <iostream>
#include "InputManager.h"

bool InputManager::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

	int buttonChanges{ m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons};
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);


	for (auto& inputBinding : m_InputBindings)
	{
		if (IsPressed(inputBinding.key))
		{
			if (inputBinding.command)
			{
				inputBinding.command->Execute();
			}
		}
	}


	return true;
}

void InputManager::AddInputBinding(const InputBinding& inputBinding)
{
	m_InputBindings.emplace_back(inputBinding);
}


bool InputManager::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}
bool InputManager::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}
bool InputManager::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}


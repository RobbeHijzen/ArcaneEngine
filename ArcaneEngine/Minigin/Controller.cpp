#include "Controller.h"

Controller::Controller(int controllerIndex)
	: m_ControllerIndex{controllerIndex}
{
}

void Controller::ProcessInput()
{
	XINPUT_STATE previousState{};

	CopyMemory(&previousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	int buttonChanges{ m_CurrentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons };
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	for (auto& inputBinding : m_InputBindings)
	{
		if (!inputBinding->command)
			continue;


		switch (inputBinding->inputType)
		{
		case InputTypeGP::IsPressed:
		{
			if (IsPressed(inputBinding->key))
			{
				inputBinding->command->Execute();
			}
			break;
		}
		case InputTypeGP::IsDownThisFrame:
		{
			if (IsDownThisFrame(inputBinding->key))
			{
				inputBinding->command->Execute();
			}
			break;
		}
		case InputTypeGP::IsUpThisFrame:
		{
			if (IsUpThisFrame(inputBinding->key))
			{
				inputBinding->command->Execute();
			}
			break;
		}
		default:
			break;
		}
	}
}




bool Controller::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}
bool Controller::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}
bool Controller::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}
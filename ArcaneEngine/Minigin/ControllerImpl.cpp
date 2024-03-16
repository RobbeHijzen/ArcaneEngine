#include "ControllerImpl.h"


void Controller::ControllerImpl::ProcessInput()
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
		case InputType::IsPressed:
		{
			if (IsPressed(inputBinding->key))
			{
				inputBinding->command->Execute();
			}
			break;
		}
		case InputType::IsDownThisFrame:
		{
			if (IsDownThisFrame(inputBinding->key))
			{
				inputBinding->command->Execute();
			}
			break;
		}
		case InputType::IsUpThisFrame:
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




bool Controller::ControllerImpl::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}
bool Controller::ControllerImpl::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}
bool Controller::ControllerImpl::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}




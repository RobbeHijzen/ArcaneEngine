#include <Windows.h>
#include <Xinput.h>

#include "Controller.h"


class Controller::ControllerImpl
{
public:

	ControllerImpl(int controllerIndex) : m_ControllerIndex{ controllerIndex } {}

	void ProcessInput();
	void BindAction(int key, InputType inputType, Command* const command)
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



Controller::Controller(int controllerIndex)
	: m_pImpl{std::make_unique<ControllerImpl>(controllerIndex)}
{
}

Controller::~Controller()
{
}

void Controller::ProcessInput()
{
	m_pImpl->ProcessInput();
}


void Controller::BindAction(int key, InputType inputType, Command* const command)
{
	m_pImpl->BindAction(key, inputType, command);
}


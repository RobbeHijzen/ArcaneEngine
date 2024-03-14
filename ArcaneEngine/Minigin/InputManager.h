#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <memory>
#include <type_traits>

#include "Singleton.h"
#include "InputCommands.h"


struct InputBinding
{
	int key;
	std::shared_ptr<Command> command;
 
	template<typename CommandType>
	InputBinding(int keyIn, CommandType commandIn)
		: key{keyIn}
	{
		static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");
		command = std::make_shared<CommandType>(commandIn);
	}
};

class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();

	void AddInputBinding(const InputBinding& inputBinding);

private:

	std::vector<InputBinding> m_InputBindings{};

	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviousState{};

	int m_ButtonsPressedThisFrame{};
	int m_ButtonsReleasedThisFrame{};


	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;
	
};




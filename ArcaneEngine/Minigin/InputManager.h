#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <memory>
#include <type_traits>
#include <WinUser.h>

#include "Singleton.h"
#include "InputCommands.h"



enum class InputType
{
	IsPressed,
	IsDownThisFrame,
	IsUpThisFrame
};
struct InputBinding
{
	int key;
	InputType inputType;
	std::shared_ptr<Command> command;
	bool usingSDL{ false };

	template<typename CommandType>
	InputBinding(int keyIn, InputType inputTypeIn, CommandType commandIn)
		: key{ keyIn }
		, inputType{inputTypeIn}
	{
		static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");
		command = std::make_shared<CommandType>(commandIn);
	}
	template<typename CommandType>
	InputBinding(SDL_Scancode keyIn, InputType inputTypeIn, CommandType commandIn)
		: key{ keyIn }
		, inputType{ inputTypeIn }
	{
		static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");
		command = std::make_shared<CommandType>(commandIn);
		usingSDL = true;
	}

};

class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();

	template<typename CommandType>
	void BindAction(int key, InputType inputType, CommandType command)
	{
		static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");
		
		InputBinding ib{ key, inputType, command };
		m_InputBindings.emplace_back(ib);
	}
	template<typename CommandType>
	void BindAction(SDL_Scancode key, InputType inputType, CommandType command)
	{
		static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");

		InputBinding ib{ key, inputType, command };
		m_InputBindings.emplace_back(ib);
	}

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




#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <memory>
#include <type_traits>
#include <WinUser.h>

#include "Singleton.h"
#include "InputCommands.h"
#include "InputHelperStructs.h"



class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();

	template<typename CommandType>
	void BindActionGP(int key, InputTypeGP inputType, CommandType command)
	{
		static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");
		
		InputBindingGP inputBinding{ key, inputType, command };
		m_InputBindingsGP.emplace_back(inputBinding);
	}
	template<typename CommandType>
	void BindActionKB(SDL_Scancode key, CommandType command)
	{
		static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");

		InputBindingKB inputBinding{ key, command };
		m_InputBindingsKB.emplace_back(inputBinding);
	}

private:

	std::vector<InputBindingGP> m_InputBindingsGP{};
	std::vector<InputBindingKB> m_InputBindingsKB{};

	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviousState{};

	int m_ButtonsPressedThisFrame{};
	int m_ButtonsReleasedThisFrame{};


	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;
	
};




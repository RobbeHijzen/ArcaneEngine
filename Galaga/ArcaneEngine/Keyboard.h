#pragma once

#include <memory>
#include "InputHelperStructs.h"

namespace AE
{
	class Keyboard
	{
	public:
		Keyboard();

		bool ProcessInput();

		void BindAction(SDL_Scancode key, InputType inputType, std::unique_ptr<Command> command)
		{
			m_InputBindings.emplace_back(std::make_unique<InputBindingKB>(key, inputType, std::move(command)));
		}

		void RemoveAllBindings() 
		{ 
			m_InputBindings.clear();
			std::fill_n(m_CurrentKeyStates.begin(), m_CurrentKeyStates.size(), false);
			std::fill_n(m_PreviousKeyStates.begin(), m_PreviousKeyStates.size(), false);
		}

	private:

		std::vector<std::unique_ptr<InputBindingKB>> m_InputBindings{};

		bool IsKeyPressed(SDL_Scancode key);
		bool IsKeyDownThisFrame(SDL_Scancode key);
		bool IsKeyUpThisFrame(SDL_Scancode key);

		void HandleKeyDown(SDL_Scancode key);
		void HandleKeyUp(SDL_Scancode key);

		std::vector<bool> m_CurrentKeyStates{};
		std::vector<bool> m_PreviousKeyStates{};
	};
}


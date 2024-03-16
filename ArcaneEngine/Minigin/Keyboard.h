#pragma once

#include <memory>
#include "InputHelperStructs.h"

class Keyboard
{
public:
	Keyboard() = default;

	void ProcessInput();

	void BindAction(SDL_Scancode key, Command* const command)
	{
		m_InputBindings.emplace_back(std::make_unique<InputBindingKB>(key, command));
	}

private:

	std::vector<std::unique_ptr<InputBindingKB>> m_InputBindings{};

};


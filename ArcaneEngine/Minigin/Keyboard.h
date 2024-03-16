#pragma once

#include <memory>
#include "InputHelperStructs.h"
#include <iostream>

class Keyboard
{
public:
	Keyboard() = default;

	bool ProcessInput();

	void BindAction(SDL_Scancode key, auto command)
	{
		m_InputBindings.emplace_back(std::make_unique<InputBindingKB>(key, command));
	}

private:

	std::vector<std::unique_ptr<InputBindingKB>> m_InputBindings{};

};


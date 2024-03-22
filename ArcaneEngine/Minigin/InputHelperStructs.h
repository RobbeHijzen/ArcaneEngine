#pragma once
#include <SDL.h>
#include <memory>
#include "Commands.h"

//////////////////////////
//*** GP = GamePad
//*** KB = KeyBoard
//////////////////////////

using namespace Commands;

enum class InputType
{
	IsPressed,
	IsDownThisFrame,
	IsUpThisFrame
};

struct InputBindingGP
{
	int key;
	InputType inputType;
	std::unique_ptr<Command> command;

	InputBindingGP(int keyIn, InputType inputTypeIn, Command* const commandIn)
		: key{ keyIn }
		, inputType{ inputTypeIn }
		, command{commandIn}
	{
	}
};

struct InputBindingKB
{
	SDL_Scancode key;
	std::unique_ptr<Command> command;

	InputBindingKB(SDL_Scancode keyIn, Command* const commandIn)
		: key{ keyIn }
		, command{commandIn}
	{
	}
};

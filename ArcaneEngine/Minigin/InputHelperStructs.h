#pragma once
#include <SDL.h>
#include <memory>
#include "InputCommands.h"

//////////////////////////
//*** GP = GamePad
//*** KB = KeyBoard
//////////////////////////

enum class InputTypeGP
{
	IsPressed,
	IsDownThisFrame,
	IsUpThisFrame
};

struct InputBindingGP
{
	int key;
	InputTypeGP inputType;
	std::shared_ptr<Command> command;

	template<typename CommandType>
	InputBindingGP(int keyIn, InputTypeGP inputTypeIn, CommandType commandIn)
		: key{ keyIn }
		, inputType{ inputTypeIn }
	{
		static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");
		command = std::make_shared<CommandType>(commandIn);
	}
};

struct InputBindingKB
{
	SDL_Scancode key;
	std::shared_ptr<Command> command;

	template<typename CommandType>
	InputBindingKB(SDL_Scancode keyIn, CommandType commandIn)
		: key{ keyIn }
	{
		static_assert(std::is_base_of<Command, CommandType>::value, "CommandType must be derived from Command");
		command = std::make_shared<CommandType>(commandIn);
	}
};

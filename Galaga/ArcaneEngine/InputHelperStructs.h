#pragma once
#include <SDL.h>
#include <memory>
#include "Command.h"

//////////////////////////
//*** GP = GamePad
//*** KB = KeyBoard
//////////////////////////

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
	std::unique_ptr<EngineCommands::Command> command;

	InputBindingGP(int keyIn, InputType inputTypeIn, std::unique_ptr<EngineCommands::Command> commandIn)
		: key{ keyIn }
		, inputType{ inputTypeIn }
		, command{std::move(commandIn)}
	{
	}
};

struct InputBindingKB
{
	SDL_Scancode key;
	InputType inputType;
	std::unique_ptr<EngineCommands::Command> command;

	InputBindingKB(SDL_Scancode keyIn, InputType inputTypeIn, std::unique_ptr<EngineCommands::Command> commandIn)
		: key{ keyIn }
		, inputType{ inputTypeIn }
		, command{std::move(commandIn)}
	{
	}
};

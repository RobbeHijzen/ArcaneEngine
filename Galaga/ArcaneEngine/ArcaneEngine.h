#pragma once
#include <string>
#include <functional>
#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>

#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "MyTime.h"

#include "Command.h"
#include "BaseComponent.h"
#include "GameObject.h"


class ArcaneEngine
{
public:
	explicit ArcaneEngine(const std::string& dataPath);
	~ArcaneEngine();
	void Run(const std::function<void()>& load);

	ArcaneEngine(const ArcaneEngine& other) = delete;
	ArcaneEngine(ArcaneEngine&& other) = delete;
	ArcaneEngine& operator=(const ArcaneEngine& other) = delete;
	ArcaneEngine& operator=(ArcaneEngine&& other) = delete;
};

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
#include "OverlapHandler.h"
#include "TimeManager.h"

#include "Command.h"
#include "BaseComponent.h"
#include "GameObject.h"

constexpr auto WINDOW_WIDTH = 640;
constexpr auto WINDOW_HEIGHT = 480;

namespace AE
{
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
}
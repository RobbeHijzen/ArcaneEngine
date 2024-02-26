#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>
#include "ArcaneEngine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Time.h"

SDL_Window* g_window{};

using namespace std::chrono;

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

ArcaneEngine::ArcaneEngine(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

ArcaneEngine::~ArcaneEngine()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void ArcaneEngine::Run(const std::function<void()>& load)
{
	load();

	// Initialize singletons
	Renderer& renderer = Renderer::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();
	InputManager& input = InputManager::GetInstance();
	Time& time = Time::GetInstance();

	// Set Time Variables
	time.Initialize(0.02f, 16);


	bool doContinue = true;
	auto lastTime{high_resolution_clock::now()};
	float lag{};

	// Main GameLoop
	while (doContinue)
	{
		// Time Management
		time.Update();
		
		lag += time.GetDeltaTime();

		// Input
		doContinue = input.ProcessInput();

		// Fixed Update
		while (lag >= time.GetFixedTimeStep())
		{
			sceneManager.FixedUpdate();
			lag -= time.GetFixedTimeStep();
		}

		// Update + Render
		sceneManager.Update();
		sceneManager.LateUpdate();

		renderer.Render();

		// Too fast? --> slow down
		const auto sleepTime{ time.GetCurrentTime() + milliseconds(time.GetMsPerFrame()) - high_resolution_clock::now()};
		std::this_thread::sleep_for(sleepTime);
	}
}

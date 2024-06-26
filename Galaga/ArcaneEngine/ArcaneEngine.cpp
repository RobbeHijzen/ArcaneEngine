#include "ArcaneEngine.h"
#include "ServiceLocator.h"
#include "AudioSystem_SDL.h"

#include <thread>

SDL_Window* g_window{};

using namespace std::chrono;
using namespace AE;

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
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
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
	ServiceLocator::ProvideAudio(std::move(std::make_unique<AudioSystem_SDL>()));

	auto audioSystem{ ServiceLocator::GetAudio()};
	std::jthread soundThread{&AudioSystem::StartSoundQueue, audioSystem};

	load();

	// Initialize singletons
	SceneManager& sceneManager = SceneManager::GetInstance();
	Renderer& renderer = Renderer::GetInstance();
	InputManager& input = InputManager::GetInstance();
	Time& time = Time::GetInstance();

	// Overlap handler
	auto overlapHandler {std::make_unique<OverlapHandler>()};

	// Set Time Variables

	constexpr bool useVsync{ true };
	SDL_GL_SetSwapInterval(useVsync);

	constexpr int maxFps{60};
	time.Initialize(0.02f, 1000 / maxFps);

	// Load all the Objects of the first scene
	sceneManager.GetCurrentScene()->Load();
	sceneManager.GameStart();

	bool doContinue = true;
	auto lastTime{high_resolution_clock::now()};
	float lag{};

	// Main GameLoop
	while (doContinue)
	{
		// Delete the flagged objects
		sceneManager.RemoveDeletedObjects();

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
		TimeManager::GetInstance().Update();
		sceneManager.LateUpdate();

		overlapHandler->CheckOverlapping();

		renderer.Render();

		// Too fast? --> slow down
		if (!useVsync)
		{
			const auto sleepTime{ time.GetCurrentTime() + milliseconds(time.GetMsPerFrame()) - high_resolution_clock::now() };
			std::this_thread::sleep_for(sleepTime);
		}
	}

	audioSystem->Stop();
	if(soundThread.joinable())
		soundThread.join();
}

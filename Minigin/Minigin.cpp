#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <chrono>

#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

SDL_Window* g_window{};

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

dae::Minigin::Minigin(const std::string &dataPath)
{
	// Init SDL
	// --------
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	// SDL Mixer
	Mix_Init(MIX_INIT_MP3);

	const int frequency{ 44'100 };
	const int channels{ 2 };
	const int chunkSize{ 2048 };
	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	// Create window
	// -------------
	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		694,
		520,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	// Init singletons
	// ---------------
	Renderer::GetInstance().Init(g_window);
	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	// Destroy singletons
	Renderer::GetInstance().Destroy();
	
	// Destroy window
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	
	// Quit SDL
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	using namespace std::chrono;

	load();
	
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	//const float fixedTimeStep{ 0.002f };	// Lower if needs to do more physics/networking

	bool doContinue = true;
	auto lastTime = high_resolution_clock::now();
	float lag = 0.f;

	const float maxFps{ 144.f };
	const float msPerFrame{ 1000.f / maxFps };

	while (doContinue)
	{
		// Calculate time
		const auto currentTime = high_resolution_clock::now();
		const float deltaTime = duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += deltaTime;

		// Input
		doContinue = input.ProcessInput(deltaTime);

		// FixedUpdate
		/*while (lag >= fixedTimeStep)
		{
			sceneManager.FixedUpdate(fixedTimeStep);
			lag -= fixedTimeStep;
		}*/

		// Update
		sceneManager.Update(deltaTime);

		// Render
		renderer.Render();

		// Sleep
		const auto sleepTime{ currentTime + milliseconds(static_cast<long long>(msPerFrame)) - high_resolution_clock::now() };
		std::this_thread::sleep_for(sleepTime);
	}
}

#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl2.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	// SDL
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	// ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render()
{
	// SDL
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	// Normal render
	SceneManager::GetInstance().Render();
	
	// ImGUI
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	// ImGUI render
	SceneManager::GetInstance().RenderImGUI();

	if (m_showDemo) ImGui::ShowDemoWindow(&m_showDemo);
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	// SDL
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	// ImGUI
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// SDL
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const utils::Rect& destRect, const utils::Rect& srcRect, float angle) const
{
	// Destination rect
	SDL_Rect dst{};
	dst.x = static_cast<int>(destRect.x);
	dst.y = static_cast<int>(destRect.y);
	dst.w = static_cast<int>(destRect.width);
	dst.h = static_cast<int>(destRect.height);
	if (destRect.width == 0 && destRect.height == 0)
	{
		SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	}

	if (srcRect != utils::Rect{})
	{
		// Source rect
		SDL_Rect src{};
		src.x = static_cast<int>(srcRect.x);
		src.y = static_cast<int>(srcRect.y);
		src.w = static_cast<int>(srcRect.width);
		src.h = static_cast<int>(srcRect.height);

		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, angle, NULL, SDL_FLIP_NONE);
	}
	else
	{
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), NULL, &dst, angle, NULL, SDL_FLIP_NONE);
	}
}

inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

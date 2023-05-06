#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "Texture2D.h"

namespace dae
{
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};

		bool m_showDemo{ false };

	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float srcLeft, float srcTop, float srcWidth, float srcHeight) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height,
						   float srcLeft, float srcTop, float srcWidth, float srcHeight) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color;
		}
	};
}


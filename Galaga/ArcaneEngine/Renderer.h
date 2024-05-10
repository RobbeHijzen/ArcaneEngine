#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "Transform.h"

namespace AE
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};

	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, glm::vec2 pos) const;
		void RenderTexture(const Texture2D& texture, AE::Rect destRect) const;
		void RenderTexture(const Texture2D& texture, AE::Rect destRect, AE::Rect sourceRect) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}



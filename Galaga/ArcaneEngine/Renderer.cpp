#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

using namespace AE;

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

void Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

}

void Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);
	SceneManager::GetInstance().Render();


	
	SDL_RenderPresent(m_renderer);
}

void Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}


void AE::Renderer::RenderTexture(const Texture2D& texture, glm::vec2 pos) const
{
	RenderTexture(texture, AE::Rect{ pos.x, pos.y }, {}, false, false);
}

void Renderer::RenderTexture(const Texture2D& texture, AE::Rect destRect, AE::Rect sourceRect, bool useDest, bool useSrc) const
{
	SDL_Rect destRectSDL{ (int)destRect.x, (int)destRect.y, (int)destRect.w, (int)destRect.h };
	SDL_Rect sourceRectSDL{ (int)sourceRect.x, (int)sourceRect.y, (int)sourceRect.w, (int)sourceRect.h };

	if (useSrc)
	{
		if (!useDest)
			SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &destRectSDL.w, &destRectSDL.h);
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &sourceRectSDL, &destRectSDL);
	}
	else
	{
		if (!useDest)
			SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &destRectSDL.w, &destRectSDL.h);
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &destRectSDL);
	}
}

SDL_Renderer* Renderer::GetSDLRenderer() const { return m_renderer; }

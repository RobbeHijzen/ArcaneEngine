#include <stdexcept>
#include <SDL_ttf.h>
#include <sstream>
#include "FpsComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Time.h"

FpsComponent::FpsComponent(std::weak_ptr<GameObject> parentGameObject, std::shared_ptr<Font> font)
	: BaseComponent(parentGameObject)
	, m_NeedsUpdate(true)
	, m_Font(std::move(font))
	, m_TextTexture(nullptr)
{ }

void FpsComponent::Update()
{
	float fps = 1.f / Time::GetInstance().GetDeltaTime();

	std::ostringstream out;
	out.precision(2);
	out << std::fixed << fps;
	SetText(std::move(out).str());

	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void FpsComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
		auto pParentGameObject{ m_pParentGameObject.lock() };

		if (pParentGameObject.get())
		{
			const auto& pos = m_RelativeTransform.GetPosition() + pParentGameObject->GetTransform().GetPosition();
			Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
		}
	}
}

// This implementation uses the "dirty flag" pattern
void FpsComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void FpsComponent::SetPosition(const float x, const float y)
{
	m_RelativeTransform.SetPosition(x, y, 0.0f);
}
#include "ImageComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

ImageComponent::ImageComponent(AE::GameObject* parentGameObject, const std::string& fileName)
	: BaseComponent(parentGameObject)
{
	m_Texture = AE::ResourceManager::GetInstance().LoadTexture(fileName);
}

void ImageComponent::SetSourceRect(SDL_Rect sourceRect)
{
	m_UseSourceRect = true;

	m_SourceRect = sourceRect;
}
void ImageComponent::SetSourceRect(int x, int y, int w, int h)
{
	SetSourceRect(SDL_Rect{x, y, w, h});
}

void ImageComponent::SetDestRect(float width, float height)
{
	m_UseDestRect = true;

	m_DestArea.x = width;
	m_DestArea.y = height;
}


void ImageComponent::Render() const
{
	AE::GameObject* pParent{ GetOwner()};

	if (pParent)
	{
		const auto& pos = m_LocalTransform.GetPosition() + pParent->GetWorldTransform().GetPosition();
		SDL_Rect destRect{ int(pos.x), int(pos.y), int(m_DestArea.x), int(m_DestArea.y)};

		AE::Renderer::GetInstance().RenderTexture(*m_Texture, destRect, m_SourceRect, m_UseDestRect, m_UseSourceRect);
	}
}

#include "ImageComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

ImageComponent::ImageComponent(AE::GameObject* parentGameObject, const std::string& fileName)
	: BaseComponent(parentGameObject)
{
	m_Texture = AE::ResourceManager::GetInstance().LoadTexture(fileName);
}

void ImageComponent::SetSourceRect(AE::Rect sourceRect)
{
	m_UseSourceRect = true;
	m_SourceRect = sourceRect;
}

void ImageComponent::SetSourcePos(float x, float y)
{
	SetSourceRect(AE::Rect{ x, y, m_SourceRect.w, m_SourceRect.h });
}

void ImageComponent::SetDestRect(AE::Rect rect)
{
	m_UseDestRect = true;
	m_DestRect = rect;
}


void ImageComponent::Render() const
{
	AE::GameObject* pParent{ GetOwner()};

	if (pParent)
	{
		const auto& parentPos = GetLocalTransform().GetPosition() + pParent->GetWorldTransform().GetPosition();
		AE::Rect destRect{ m_DestRect};
		destRect.x += parentPos.x;
		destRect.y += parentPos.y;

		if(m_UseSourceRect)
			AE::Renderer::GetInstance().RenderTexture(*m_Texture, destRect, m_SourceRect);
		else if(m_UseDestRect)
			AE::Renderer::GetInstance().RenderTexture(*m_Texture, destRect);
		else
			AE::Renderer::GetInstance().RenderTexture(*m_Texture, glm::vec2{destRect.x, destRect.y});
	}
}

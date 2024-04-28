#include "ImageComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

ImageComponent::ImageComponent(GameObject* parentGameObject, const std::string& fileName, float destWidth, float destHeight)
	: BaseComponent(parentGameObject)
	, m_DestWidth{destWidth}
	, m_DestHeight{destHeight}
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
}

ImageComponent::ImageComponent(GameObject* parentGameObject, const std::string& fileName)
	: ImageComponent(parentGameObject, fileName, 0.f, 0.f)
{
	m_UseDestSizes = false;
}

void ImageComponent::Update()
{
}

void ImageComponent::Render() const
{
	GameObject* pParent{ GetOwner()};

	if (pParent)
	{
		const auto& pos = m_LocalTransform.GetPosition() + pParent->GetWorldTransform().GetPosition();

		if (m_UseDestSizes)
		{
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, m_DestWidth, m_DestHeight);
		}
		else
		{
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
		}
	}
}

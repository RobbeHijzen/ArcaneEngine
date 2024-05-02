#include "ImageComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

ImageComponent::ImageComponent(AE::GameObject* parentGameObject, const std::string& fileName, glm::vec2 sourceArea, glm::vec2 destArea)
	: BaseComponent(parentGameObject)
	, m_DestArea{ destArea }
	, m_SourceArea{ sourceArea }
{
	m_Texture = AE::ResourceManager::GetInstance().LoadTexture(fileName);
}

ImageComponent::ImageComponent(AE::GameObject* parentGameObject, const std::string& fileName, glm::vec2 sourceArea)
	: ImageComponent(parentGameObject, fileName, sourceArea, {})
{
	m_UseDestSizes = false;
}


ImageComponent::ImageComponent(AE::GameObject* parentGameObject, const std::string& fileName)
	: ImageComponent(parentGameObject, fileName, {}, {})
{
	m_UseDestSizes = false;
	m_UseSourceSizes = false;
}

void ImageComponent::Update()
{
}

void ImageComponent::Render() const
{
	AE::GameObject* pParent{ GetOwner()};

	if (pParent)
	{
		const auto& pos = m_LocalTransform.GetPosition() + pParent->GetWorldTransform().GetPosition();

		if (m_UseDestSizes)
		{
			AE::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, m_DestArea);
		}
		else
		{
			AE::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
		}
	}
}

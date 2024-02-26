#include "ImageComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

ImageComponent::ImageComponent(GameObject* parentGameObject, const std::string& fileName)
	: BaseComponent(parentGameObject)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
}

void ImageComponent::Update()
{
}

void ImageComponent::Render() const
{
	GameObject* pParent{ GetParent()};

	if (pParent)
	{
		const auto& pos = m_LocalTransform.GetPosition() + pParent->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

#include "ImageComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

ImageComponent::ImageComponent(std::weak_ptr<GameObject> parentGameObject, const std::string& fileName)
	: BaseComponent(parentGameObject)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
}

void ImageComponent::Update()
{
}

void ImageComponent::Render() const
{
	auto pParentGameObject{ m_pParentGameObject.lock() };

	if (pParentGameObject.get())
	{
		const auto& pos = m_RelativeTransform.GetPosition() + pParentGameObject->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

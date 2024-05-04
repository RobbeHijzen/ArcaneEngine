#include "HitboxComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"

HitboxComponent::HitboxComponent(AE::GameObject* parentGameObject, float width, float height)
	: BaseComponent(parentGameObject)
{
	m_Hitbox.w = width;
	m_Hitbox.h = height;

	m_Texture = AE::ResourceManager::GetInstance().LoadTexture("outlinedBox.png");
}

void HitboxComponent::Update()
{
	const auto& localPos{ GetLocalTransform().GetPosition() };
	const auto& objectWorldPos{GetOwner()->GetWorldTransform().GetPosition()};

	auto pos{ localPos + objectWorldPos };

	m_Hitbox.x = pos.x;
	m_Hitbox.y = pos.y;
}

void HitboxComponent::Render() const
{
	AE::Renderer::GetInstance().RenderTexture(*m_Texture, m_Hitbox, {}, true, false);
}

void HitboxComponent::OnOverlap(HitboxComponent* otherComp)
{
	if (UsesIgnoreTag(m_IgnoreTags, otherComp->GetOwner())) return;
	if (UsesIgnoreTag(otherComp->m_IgnoreTags, GetOwner())) return;
	

	m_LatestOverlapComp = otherComp;
	GetOwner()->NotifyAll(AE::Event::OnOverlap);
}

bool HitboxComponent::UsesIgnoreTag(std::vector<std::string> ignoreTags, AE::GameObject* go)
{
	for (auto& tag : ignoreTags)
	{
		if (go->HasTag(tag))
			return true;
	}
	return false;
}

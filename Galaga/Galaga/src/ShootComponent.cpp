#include "ShootComponent.h"
#include "Observers.h"

#include "SceneManager.h"

#include "ProjectileMovementComponent.h"
#include "ImageComponent.h"

ShootComponent::ShootComponent(AE::GameObject* pParent, glm::vec2 direction, float bulletSpeed)
	: BaseComponent(pParent)
	, m_BulletDirection{ direction }
	, m_BulletSpeed{ bulletSpeed }
{
	auto observer{ std::make_unique<BulletObserver>()};
	pParent->AddObserver(std::move(observer));
}

void ShootComponent::FireBullet()
{
	auto bullet = std::make_shared<AE::GameObject>();

	auto imageComp{ std::make_shared<ImageComponent>(bullet.get(), "Galaga/GalagaBullet.png")};
	imageComp->SetDestRect(55.f, 55.f);
	bullet->AddComponent(imageComp);
	bullet->AddComponent(std::make_shared<ProjectileMovementComponent>(bullet.get(), m_BulletDirection, m_BulletSpeed, 5.f));

	bullet->SetLocalTransform(GetOwner()->GetWorldTransform());

	auto& scene{ AE::SceneManager::GetInstance()};
	scene.GetCurrentScene()->Add(bullet);
}

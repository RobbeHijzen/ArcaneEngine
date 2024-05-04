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
	auto observer{ std::make_unique<ShootingObserver>()};
	pParent->AddObserver(std::move(observer));
}

void ShootComponent::FireBullet()
{
	auto bullet = std::make_shared<AE::GameObject>();
	bullet->SetLocalTransform(GetOwner()->GetWorldTransform());

	auto imageComp{ std::make_shared<ImageComponent>(bullet.get(), "Galaga2.png")};
	imageComp->SetDestRect(35.f, 35.f);
	imageComp->SetSourceRect(307, 136, 16, 16);
	bullet->AddComponent(imageComp);
	bullet->AddLocalTransform(AE::Transform{ 7.5f, -17.f }); // Position Offset

	// Components
	bullet->AddComponent(std::make_shared<ProjectileMovementComponent>(bullet.get(), m_BulletDirection, m_BulletSpeed, 5.f));
	
	auto hitboxComp{ std::make_shared<HitboxComponent>(bullet.get(), 17.f, 25.f)};
	hitboxComp->AddIgnoreTag("Friendly");
	hitboxComp->AddLocalPosition(7.5f, 0.f ); // Position Offset

	bullet->AddComponent(hitboxComp);

	bullet->AddObserver(std::move(std::make_unique<BulletObserver>()));


	auto& scene{ AE::SceneManager::GetInstance()};
	scene.GetCurrentScene()->Add(bullet);

	// Notify observers
	GetOwner()->NotifyAll(AE::Event::FireBullet);
}

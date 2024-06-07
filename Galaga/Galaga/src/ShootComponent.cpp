#include "ShootComponent.h"
#include "Observers.h"

#include "SceneManager.h"

#include "ProjectileMovementComponent.h"
#include "ImageComponent.h"

ShootComponent::ShootComponent(AE::GameObject* pParent)
	: BaseComponent(pParent)
{
	auto observer{ std::make_unique<ShootingObserver>()};
	pParent->AddObserver(std::move(observer));
}

void ShootComponent::FireBullet()
{
	if (m_AmountOfLiveBullets >= m_MaxAmountOfBullets) return;

	auto bullet = std::make_shared<AE::GameObject>();
	bullet->SetLocalTransform(GetOwner()->GetWorldTransform());
	bullet->AddLocalTransform(AE::Transform{ m_BulletOffset });
	bullet->AddTag("Bullet");

	// Image Component
	auto imageComp{ std::make_shared<ImageComponent>(bullet.get(), "Galaga.png")};
	imageComp->SetDestRect({ 0, 0, 35.f, 35.f });
	imageComp->SetSourceRect({ 307, 136, 16, 16 });
	bullet->AddComponent(imageComp);

	// Projectile Component
	std::shared_ptr<ProjectileMovementComponent> projComp{};
	if (m_UseSeekTarget)
	{
		int randomIndex{rand() % int(m_SeekTargets.size())};

		glm::vec2 dir{m_SeekTargets[randomIndex]->GetWorldTransform().GetPosition() - bullet->GetWorldTransform().GetPosition()};
		projComp = std::make_shared<ProjectileMovementComponent>(bullet.get(), dir, m_BulletSpeed, m_BulletLifetime);
	}
	else
	{
		projComp = std::make_shared<ProjectileMovementComponent>(bullet.get(), m_BulletDirection, m_BulletSpeed, m_BulletLifetime);
	}

	bullet->AddComponent(projComp);
	
	// Hitbox Component
	auto hitboxComp{ std::make_shared<HitboxComponent>(bullet.get(), 17.f, 25.f)};
	for (const auto& ignoreTag : m_IgnoreTags)
	{
		hitboxComp->AddIgnoreTag(ignoreTag);
	}
	hitboxComp->AddLocalPosition(7.5f, 0.f ); // Hitbox Offset (to allign with the sprite)

	bullet->AddComponent(hitboxComp);

	// Observers
	bullet->AddObserver(std::move(std::make_unique<BulletObserver>()));
	bullet->AddObserver(std::move(std::make_unique<SpawnedBulletObserver>(this)));

	// Spawn into Scene
	auto& scene{ AE::SceneManager::GetInstance()};
	scene.GetCurrentScene()->Add(bullet);

	// Notify observers
	GetOwner()->NotifyAll(AE::Event::FireBullet);
	++m_AmountOfLiveBullets;
}

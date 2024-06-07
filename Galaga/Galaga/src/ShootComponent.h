#pragma once

#include "BaseComponent.h"
#include "GameObject.h"


class ShootComponent : public AE::BaseComponent
{
public:

	ShootComponent(AE::GameObject* pParent);

	void FireBullet();

	void SetBulletSpeed(float bulletSpeed) { m_BulletSpeed = bulletSpeed; }
	void SetBulletDirection(glm::vec2 dir) { m_BulletDirection = dir; }

	void AddSeekTarget(AE::GameObject* seekTarget) { m_UseSeekTarget = true; m_SeekTargets.emplace_back(seekTarget); }

	void SetBulletSpawnOffset(glm::vec2 offset) { m_BulletOffset = offset; }
	void AddIgnoreTag(const std::string& ignoreTag) { m_IgnoreTags.emplace_back(ignoreTag); }

	void RemoveBullet() { --m_AmountOfLiveBullets; }

private:

	float m_BulletSpeed{300.f};
	float m_BulletLifetime{5.f};

	bool m_UseSeekTarget{ false };

	glm::vec2 m_BulletDirection{0.f, -1.f}; // used without seek target
	std::vector<AE::GameObject*> m_SeekTargets{}; // used with seek target

	glm::vec2 m_BulletOffset{};

	std::vector<std::string> m_IgnoreTags{};

	int m_AmountOfLiveBullets{};
	int m_MaxAmountOfBullets{ 2 };
};


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

	void SetSeekTarget(AE::GameObject* seekTarget) { m_UseSeekTarget = true; m_SeekTarget = seekTarget; }

	void SetBulletSpawnOffset(glm::vec2 offset) { m_BulletOffset = offset; }
	void AddIgnoreTag(const std::string& ignoreTag) { m_IgnoreTags.emplace_back(ignoreTag); }

private:

	float m_BulletSpeed{300.f};

	bool m_UseSeekTarget{ false };

	glm::vec2 m_BulletDirection{0.f, -1.f}; // used without seek target
	AE::GameObject* m_SeekTarget{}; // used with seek target

	glm::vec2 m_BulletOffset{};

	std::vector<std::string> m_IgnoreTags{};
};


#pragma once

#include "BaseComponent.h"
#include "GameObject.h"


class ShootComponent : public AE::BaseComponent
{
public:

	ShootComponent(AE::GameObject* pParent, glm::vec2 direction, float bulletSpeed = 150.f);

	void FireBullet();

private:

	float m_BulletSpeed;
	glm::vec2 m_BulletDirection;

};


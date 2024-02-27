#pragma once
#include <string>
#include <memory>
#include "GameObject.h"


class RotatingComponent final : public BaseComponent
{
public:

	RotatingComponent(GameObject* const parentGameObject, glm::vec2 center, float radius, float rotationSpeed = 1.f);

	virtual ~RotatingComponent() = default;
	RotatingComponent(const RotatingComponent& other) = delete;
	RotatingComponent(RotatingComponent&& other) = delete;
	RotatingComponent& operator=(const RotatingComponent& other) = delete;
	RotatingComponent& operator=(RotatingComponent&& other) = delete;

	virtual void Update() override;

private:

	glm::vec2 m_Center{};
	const float m_Radius{};

	float m_CurrentAngle{}; // in radians
	float m_RotationSpeed{}; // radians per second
};


#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "GameObject.h"

class MovementComponent final : public BaseComponent
{
public:

	MovementComponent(GameObject* const parentGameObject, float movementSpeed);

	virtual ~MovementComponent() = default;
	MovementComponent(const MovementComponent& other) = delete;
	MovementComponent(MovementComponent&& other) = delete;
	MovementComponent& operator=(const MovementComponent& other) = delete;
	MovementComponent& operator=(MovementComponent&& other) = delete;


	void Move(glm::vec2 direction);

private:

	const float m_MovementSpeed;
};


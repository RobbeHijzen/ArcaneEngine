#pragma once

#include "GameObject.h"

namespace AE
{
	namespace StateHelpers
	{
		bool HasReachedPoint(glm::vec2 seekPos, AE::GameObject* gameObject, float acceptanceRadius = 1.f);
		glm::vec2 GetDirection(glm::vec2 seekPos, AE::GameObject* gameObject);
	}
}
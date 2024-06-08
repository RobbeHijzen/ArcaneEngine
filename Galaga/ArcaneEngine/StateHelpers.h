#pragma once

#include "GameObject.h"

namespace AE
{
	namespace StateHelpers
	{
		bool HasReachedPoint(glm::vec2 seekPos, AE::GameObject* gameObject, float acceptanceRadius = 2.f);
		glm::vec2 GetDirection(glm::vec2 seekPos, AE::GameObject* gameObject);
	}
}
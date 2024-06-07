#pragma once

#include "HitboxComponent.h"
#include <queue>

namespace AE
{
	class OverlapHandler
	{
	public:

		void CheckOverlapping();

	private:

		std::queue<std::shared_ptr<AE::GameObject>> GetAllChildrenOf(std::shared_ptr<AE::GameObject> go);
		bool AreOverlapping(HitboxComponent* hitbox1, HitboxComponent* hitbox2);
	};
}


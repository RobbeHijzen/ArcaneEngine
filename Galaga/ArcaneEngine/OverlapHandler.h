#pragma once

#include "HitboxComponent.h"

namespace AE
{
	class OverlapHandler
	{
	public:

		void CheckOverlapping();

	private:

		bool AreOverlapping(HitboxComponent* hitbox1, HitboxComponent* hitbox2);
	};
}


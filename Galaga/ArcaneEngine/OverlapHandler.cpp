#include "OverlapHandler.h"

#include "SceneManager.h"
#include "Scene.h"

using namespace AE;

void OverlapHandler::CheckOverlapping()
{
	auto gameObjects{SceneManager::GetInstance().GetCurrentScene()->GetGameObjects()};

	for (int i{}; i < gameObjects.size(); ++i)
	{
		auto go1{gameObjects[i]};

		if (auto hitboxComp1 = go1->GetComponent<HitboxComponent>())
		{
			if (!hitboxComp1->IsActive()) continue;

			for (int j{ i + 1 }; j < gameObjects.size(); ++j)
			{
				auto go2{ gameObjects[j] };

				if (auto hitboxComp2 = go2->GetComponent<HitboxComponent>())
				{
					if (!hitboxComp2->IsActive()) continue;

					// If here then both GameObjects have a HitboxComponent

					if (AreOverlapping(hitboxComp1.get(), hitboxComp2.get()))
					{
						hitboxComp1->OnOverlap(hitboxComp2.get());
						hitboxComp2->OnOverlap(hitboxComp1.get());
					}
				}
			}
		}
	}
}

bool AE::OverlapHandler::AreOverlapping(HitboxComponent* hitbox1, HitboxComponent* hitbox2)
{
	const AE::Rect& rectA{hitbox1->GetHitbox()};
	const AE::Rect& rectB{hitbox2->GetHitbox()};

	return
		rectA.x + rectA.w >= rectB.x &&
		rectB.x + rectB.w >= rectA.x &&
		rectA.y + rectA.h >= rectB.y &&
		rectB.y + rectB.h >= rectA.y;
}

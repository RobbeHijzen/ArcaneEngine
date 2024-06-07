#include "OverlapHandler.h"

#include "SceneManager.h"
#include "Scene.h"

using namespace AE;

void OverlapHandler::CheckOverlapping()
{
	auto gameObjects{SceneManager::GetInstance().GetCurrentScene()->GetGameObjects()};
	if (gameObjects.size() < 2) return;

	for (int i{}; i < gameObjects.size(); ++i)
	{
		auto gos1{GetAllChildrenOf(gameObjects[i])};
		while (!gos1.empty())
		{
			auto go1 = gos1.front();
			gos1.pop();

			if (auto hitboxComp1 = go1->GetComponent<HitboxComponent>())
			{
				if (!hitboxComp1->IsActive()) continue;

				for (int j{ i + 1 }; j < gameObjects.size(); ++j)
				{
					auto gos2{ GetAllChildrenOf(gameObjects[j]) };
					while (!gos2.empty())
					{
						auto go2 = gos2.front();
						gos2.pop();

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
	}
}

std::queue<std::shared_ptr<AE::GameObject>> OverlapHandler::GetAllChildrenOf(std::shared_ptr<AE::GameObject> go)
{
	std::queue<std::shared_ptr<AE::GameObject>> queue{};
	queue.push(go);
	for (const auto& child : go->GetChildren())
	{
		auto otherQueue{ GetAllChildrenOf(child) };
		while (!otherQueue.empty())
		{
			queue.push(otherQueue.front());
			otherQueue.pop();
		}
	}

	return queue;
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

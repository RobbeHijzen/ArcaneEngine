#include "NoOffScreenComponent.h"
#include "ArcaneEngine.h"

void NoOffScreenComponent::Update()
{
	auto hitbox{ m_HitboxComp->GetHitbox() };
	if (hitbox.x < 0.f)
	{
		GetOwner()->AddLocalTransform({ -hitbox.x, 0.f });
	}
	else if (hitbox.x + hitbox.w > WINDOW_WIDTH)
	{
		GetOwner()->AddLocalTransform({ WINDOW_WIDTH - (hitbox.x + hitbox.w), 0.f });
	}
	if (hitbox.y < 0.f)
	{
		GetOwner()->AddLocalTransform({ 0.f, -hitbox.y });
	}
	else if (hitbox.y + hitbox.h > WINDOW_HEIGHT)
	{
		GetOwner()->AddLocalTransform({0.f, WINDOW_HEIGHT - (hitbox.y + hitbox.h)});
	}
}

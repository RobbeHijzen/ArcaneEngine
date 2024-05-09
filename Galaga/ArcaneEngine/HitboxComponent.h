#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "Transform.h"

#include <SDL.h>

class HitboxComponent : public AE::BaseComponent
{
public:
	HitboxComponent(AE::GameObject* parentGameObject, float width, float height);

	void Update() override;
	void OnOverlap(HitboxComponent* otherComp);

	const AE::Rect& GetHitbox() const { return m_Hitbox; }

	auto GetLatestOverlapGO() const { return m_LatestOverlapComp->GetOwner(); }
	void AddIgnoreTag(std::string tag) { m_IgnoreTags.emplace_back(tag); }

	bool IsActive() const { return m_IsActive; }
	void SetActive(bool active) { m_IsActive = active; }

private:

	AE::Rect m_Hitbox{};

	HitboxComponent* m_LatestOverlapComp{};
	std::vector<std::string> m_IgnoreTags{};

	bool m_IsActive{ true };

	bool UsesIgnoreTag(std::vector<std::string> ignoreTags, AE::GameObject* go);


	// For debug drawing the outlines of the hitbox
public:
	void Render() const override;
private:
	std::shared_ptr<AE::Texture2D> m_Texture{};

};


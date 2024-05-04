#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "ImageComponent.h"

#include <memory>

class ScrollingImageComponent : public AE::BaseComponent
{
public:

	ScrollingImageComponent(AE::GameObject* parentGameObject, const std::string& fileName, float scrollingSpeed);

	virtual ~ScrollingImageComponent() = default;
	ScrollingImageComponent(const ScrollingImageComponent& other) = delete;
	ScrollingImageComponent(ScrollingImageComponent&& other) = delete;
	ScrollingImageComponent& operator=(const ScrollingImageComponent& other) = delete;
	ScrollingImageComponent& operator=(ScrollingImageComponent&& other) = delete;

	void SetSourceRect(AE::Rect sourceRect);
	void SetSourceRect(float x, float y, float w, float h);
	void SetDestRect(float width, float height);

	void Update() override;

private:

	float m_ScrollingSpeed{};
	int m_ScrollSign{};

	float m_DestHeight{};

	std::shared_ptr<ImageComponent> m_FirstImage{};
	std::shared_ptr<ImageComponent> m_SecondImage{};
};


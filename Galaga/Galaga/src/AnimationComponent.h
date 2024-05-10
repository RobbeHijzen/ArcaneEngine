#pragma once
#include "ImageComponent.h"


class Texture2D;

class AnimationComponent final : public AE::BaseComponent
{
public:

	AnimationComponent(AE::GameObject* parentGameObject, const std::string& fileName, int colsAmount, float animTime = 0.5f);

	virtual ~AnimationComponent() = default;
	AnimationComponent(const AnimationComponent& other) = delete;
	AnimationComponent(AnimationComponent&& other) = delete;
	AnimationComponent& operator=(const AnimationComponent& other) = delete;
	AnimationComponent& operator=(AnimationComponent&& other) = delete;

	void SetSourceRect(AE::Rect sourceRect) 
	{
		m_OriginalSourceRect = sourceRect;
		m_ImageComp->SetSourceRect(sourceRect); 
	}
	void SetSourcePos(glm::vec2 pos)
	{ 
		m_OriginalSourceRect.x = pos.x; 
		m_OriginalSourceRect.y = pos.y; 
		m_ImageComp->SetSourcePos(pos); 
	}
	void SetDestRect(AE::Rect rect) { m_ImageComp->SetDestRect(rect); }

	void Update() override;

private:
	std::shared_ptr<ImageComponent> m_ImageComp{};

	AE::Rect m_OriginalSourceRect{};

	float m_AnimTime; // Time between every image
	float m_CurrentTime{};

	int m_AmountCols;
	int m_ColIndex{};
};





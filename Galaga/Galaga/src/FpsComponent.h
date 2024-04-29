#pragma once
#include <string>
#include <memory>
#include "GameObject.h"

class TextComponent;

class FpsComponent final : public AE::BaseComponent
{
public:

	FpsComponent(AE::GameObject* const parentGameObject);

	virtual ~FpsComponent() = default;
	FpsComponent(const FpsComponent& other) = delete;
	FpsComponent(FpsComponent&& other) = delete;
	FpsComponent& operator=(const FpsComponent& other) = delete;
	FpsComponent& operator=(FpsComponent&& other) = delete;

	virtual void GameStart() override;
	virtual void Update() override;

private:

	int m_FrameCount{};
	float m_CurrentDelay{};
	const float m_MaxDelay{0.2f};

	TextComponent* m_pTextComponent{};
};


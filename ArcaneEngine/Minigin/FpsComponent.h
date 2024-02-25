#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"


class Font;
class Texture2D;


class FpsComponent final : public BaseComponent
{
public:

	FpsComponent(std::weak_ptr<GameObject> parentGameObject, std::shared_ptr<Font> font);

	virtual ~FpsComponent() = default;
	FpsComponent(const FpsComponent& other) = delete;
	FpsComponent(FpsComponent&& other) = delete;
	FpsComponent& operator=(const FpsComponent& other) = delete;
	FpsComponent& operator=(FpsComponent&& other) = delete;


	virtual void Update() override;
	virtual void Render() const override;

	void SetText(const std::string& text);
	void SetPosition(float x, float y);


private:

	bool m_NeedsUpdate;
	std::string m_Text{};
	std::shared_ptr<Font> m_Font;
	std::shared_ptr<Texture2D> m_TextTexture;
};


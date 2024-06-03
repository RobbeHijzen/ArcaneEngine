#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"
#include <SDL.h>

class Texture2D;

class ImageComponent final : public AE::BaseComponent
{
public:

	ImageComponent(AE::GameObject* parentGameObject, const std::string& fileName);

	virtual ~ImageComponent() = default;
	ImageComponent(const ImageComponent& other) = delete;
	ImageComponent(ImageComponent&& other) = delete;
	ImageComponent& operator=(const ImageComponent& other) = delete;
	ImageComponent& operator=(ImageComponent&& other) = delete;

	void SetSourceRect(AE::Rect sourceRect);
	void SetSourcePos(glm::vec2 pos);
	void SetDestRect(AE::Rect rect);

	void Render() const override;

	AE::Rect GetSourceRect() const { return m_SourceRect; }

private:

	std::shared_ptr<AE::Texture2D> m_Texture{};

	bool m_UseDestRect{ false };
	bool m_UseSourceRect{ false };

	AE::Rect m_DestRect{};
	AE::Rect m_SourceRect{};
};





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
	void SetSourceRect(float x, float y, float w, float h);
	void SetDestRect(float width, float height);

	void Render() const override;

private:

	std::shared_ptr<AE::Texture2D> m_Texture{};

	bool m_UseDestRect{ false };
	bool m_UseSourceRect{ false };

	glm::vec2 m_DestArea{};
	AE::Rect m_SourceRect{};
};





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

	void SetSourceRect(SDL_Rect sourceRect);
	void SetSourceRect(int x, int y, int w, int h);
	void SetDestRect(float width, float height);

	void Render() const override;

private:

	std::shared_ptr<AE::Texture2D> m_Texture{};

	bool m_UseDestRect{ false };
	bool m_UseSourceRect{ false };

	glm::vec2 m_DestArea{};
	SDL_Rect m_SourceRect{};
};





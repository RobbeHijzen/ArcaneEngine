#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"

class Texture2D;

class ImageComponent final : public AE::BaseComponent
{
public:

	ImageComponent(AE::GameObject* parentGameObject, const std::string& fileName, glm::vec2 sourceArea, glm::vec2 destArea);
	ImageComponent(AE::GameObject* parentGameObject, const std::string& fileName, glm::vec2 sourceArea);
	ImageComponent(AE::GameObject* parentGameObject, const std::string& fileName);

	virtual ~ImageComponent() = default;
	ImageComponent(const ImageComponent& other) = delete;
	ImageComponent(ImageComponent&& other) = delete;
	ImageComponent& operator=(const ImageComponent& other) = delete;
	ImageComponent& operator=(ImageComponent&& other) = delete;


	void Update() override;
	void Render() const override;

private:

	std::shared_ptr<AE::Texture2D> m_Texture{};

	bool m_UseDestSizes{ true };
	bool m_UseSourceSizes{ true };

	glm::vec2 m_DestArea{};
	glm::vec2 m_SourceArea{};
};





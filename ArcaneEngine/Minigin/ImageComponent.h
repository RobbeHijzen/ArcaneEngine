#pragma once
#include <string>
#include <memory>
#include "GameObject.h"
#include "Transform.h"

class Texture2D;

class ImageComponent final : public BaseComponent
{
public:

	ImageComponent(GameObject* parentGameObject, const std::string& fileName);

	virtual ~ImageComponent() = default;
	ImageComponent(const ImageComponent& other) = delete;
	ImageComponent(ImageComponent&& other) = delete;
	ImageComponent& operator=(const ImageComponent& other) = delete;
	ImageComponent& operator=(ImageComponent&& other) = delete;


	void Update() override;
	void Render() const override;

private:

	std::shared_ptr<Texture2D> m_Texture{};
};





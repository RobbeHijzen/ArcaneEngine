#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "BaseComponent.h"

namespace AE
{
	class Font;
	class Texture2D;
	class GameObject;
}



class TextComponent final : public AE::BaseComponent
{
public:

	TextComponent(AE::GameObject* parentGameObject, const std::string& text, std::shared_ptr<AE::Font> font, glm::u8vec3 color = {255, 255, 255});
	
	virtual ~TextComponent() = default;
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;


	virtual void Update() override;
	virtual void Render() const override;

	void SetText(const std::string& text);
	void SetPosition(float x, float y);


private:

	bool m_NeedsUpdate;
	std::string m_Text;
	std::shared_ptr<AE::Font> m_Font;
	std::shared_ptr<AE::Texture2D> m_TextTexture;

	glm::u8vec3 m_Color{};
};


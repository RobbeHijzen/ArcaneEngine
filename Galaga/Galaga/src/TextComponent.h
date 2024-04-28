#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "BaseComponent.h"

class Font;
class Texture2D;
class GameObject;


class TextComponent final : public BaseComponent
{
public:

	TextComponent(GameObject* parentGameObject, const std::string& text, std::shared_ptr<Font> font);
	
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
	std::shared_ptr<Font> m_Font;
	std::shared_ptr<Texture2D> m_TextTexture;
};


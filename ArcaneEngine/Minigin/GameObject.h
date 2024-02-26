#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "BaseComponent.h"


class Texture2D;

// todo: this should become final.
class GameObject final
{
public:

	GameObject() = default;

	virtual ~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;


	bool AddComponent(std::shared_ptr<BaseComponent> component);

	template<typename T>
	std::shared_ptr<T> GetComponent();
	template<typename T>
	void RemoveComponent();

	void ClearComponents() { m_Components.clear(); }

	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render() const;

	void SetPosition(float x, float y);
	Transform GetTransform() { return m_Transform; }

private:

	Transform m_Transform{};
	std::vector <std::shared_ptr<BaseComponent>> m_Components{};
};


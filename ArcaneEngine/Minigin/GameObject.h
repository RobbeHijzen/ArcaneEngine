#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "BaseComponent.h"


class Texture2D;

class GameObject final
{
public:

	GameObject() = default;

	~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;


	void Initialize();

	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render() const;

	// Component Management
	bool AddComponent(std::shared_ptr<BaseComponent> component);

	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		for (auto& component : m_Components)
		{
			if (auto castedComponent = std::dynamic_pointer_cast<T>(component))
			{
				return castedComponent;
			}
		}
		return nullptr;
	}
	template<typename T>
	void RemoveComponent()
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if (std::dynamic_pointer_cast<T>(*it))
			{
				m_Components.erase(it);
				return;
			}
		}
	}

	void ClearComponents() { m_Components.clear(); }

	// Parent Management
	void SetParent(GameObject* parent);
	GameObject* GetParent() { return m_Parent; }

	// Children Management
	size_t GetChildCount() { return m_Children.size(); }
	GameObject* GetChildAt(unsigned int index) { assert(index < m_Children.size()); return m_Children[index]; };

	void RemoveChild(GameObject* child);
	void AddChild(GameObject* child);

	// Transforms
	void SetLocalTransform(float x, float y);
	Transform GetLocalTransform() { return m_LocalTransform; }

private:

	Transform m_LocalTransform{};
	Transform m_WorldTransform{};

	std::vector<std::shared_ptr<BaseComponent>> m_Components{};
		
	GameObject* m_Parent{};
	std::vector<GameObject*> m_Children{};

	bool IsChild(GameObject* gameObject);
};


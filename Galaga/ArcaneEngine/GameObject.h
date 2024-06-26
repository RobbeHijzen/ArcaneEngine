#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "BaseComponent.h"
#include "Subject.h"

namespace AE
{
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


		void GameStart();

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
		void SetParent(GameObject* parent, bool keepWorldPosition = true);
		GameObject* GetParent() const { return m_Parent; }

		// Children Management
		size_t GetChildCount() const { return m_Children.size(); }
		std::shared_ptr<GameObject> GetChildAt(unsigned int index) const { assert(index < m_Children.size()); return m_Children[index]; };
		std::shared_ptr<GameObject> GetChildSharedPtr(GameObject* child) const;
		const std::vector<std::shared_ptr<GameObject>>& GetChildren() const { return m_Children; }

		// Transforms
		void SetLocalTransform(Transform transform);
		void AddLocalTransform(Transform transform);

		Transform GetLocalTransform() const { return m_LocalTransform; }
		void SetWorldTransformDirty();

		Transform GetWorldTransform();
		Transform GetSpawnTransform() const { return m_SpawnTransform; }

		// Deleting
		void Delete();
		bool IsDeleted() const { return m_IsDeleted; }


		void AddObserver(std::unique_ptr<Observer> observer)
		{
			m_pSubject->AddObserver(std::move(observer));
		}
		void RemoveObserver(Observer* observer)
		{
			m_pSubject->RemoveObserver(observer);
		}

		void NotifyAll(Event event) { m_pSubject->NotifyAll(event, this); }

		void AddTag(std::string tag) { m_Tags.emplace_back(tag); }
		bool HasTag(std::string tag)
		{
			auto it = std::find(m_Tags.begin(), m_Tags.end(), tag); 
			return it != m_Tags.end();
		}

		bool IsVisible() const { return m_IsVisible; }
		void SetVisible(bool isVisible) { m_IsVisible = isVisible; }

	private:

		bool m_IsDeleted{ false };
		bool m_IsVisible{ true };

		Transform m_SpawnTransform{};

		Transform m_LocalTransform{};
		Transform m_WorldTransform{};
		bool m_IsTransformDirty{};

		std::vector<std::shared_ptr<BaseComponent>> m_Components{};

		GameObject* m_Parent{};
		std::vector<std::shared_ptr<GameObject>> m_Children{};


		void AddChild(std::shared_ptr<GameObject>child);
		void RemoveChild(GameObject* child);

		void UpdateWorldTransform();

		bool IsChild(GameObject* gameObject);

		std::unique_ptr<Subject> m_pSubject{ std::make_unique<Subject>() };

		std::vector<std::string> m_Tags{};
	};
}

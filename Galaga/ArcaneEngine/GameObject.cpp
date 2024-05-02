#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

namespace AE
{
	GameObject::~GameObject()
	{
	}


	bool GameObject::AddComponent(std::shared_ptr<BaseComponent> component)
	{
		if (component.get())
		{
			m_Components.emplace_back(component);
			return true;
		}
		return false;
	}


	void GameObject::GameStart()
	{
		for (auto& component : m_Components)
		{
			component->GameStart();
		}
		for (auto& child : m_Children)
		{
			child->GameStart();
		}

		NotifyAll(Event::GameStart);
	}

	void GameObject::Update()
	{

		for (auto& component : m_Components)
		{
			component->Update();
		}

		for (auto& child : m_Children)
		{
			child->Update();
		}
	}

	void GameObject::FixedUpdate()
	{
		for (auto& component : m_Components)
		{
			component->FixedUpdate();
		}
		for (auto& child : m_Children)
		{
			child->FixedUpdate();
		}
	}

	void GameObject::LateUpdate()
	{
		for (auto& component : m_Components)
		{
			component->LateUpdate();
		}
		for (auto& child : m_Children)
		{
			child->LateUpdate();
		}
	}

	void GameObject::Render() const
	{
		for (auto& component : m_Components)
		{
			component->Render();
		}
		for (auto& child : m_Children)
		{
			child->Render();
		}

	}

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
		// Check for valid parent
		if (IsChild(parent) || parent == this || parent == m_Parent)
		{
			return;
		}

		// Transform Handling
		if (keepWorldPosition)
		{
			if (parent == nullptr)
			{
				SetLocalTransform(GetWorldTransform());
			}
			else
			{
				SetLocalTransform(GetWorldTransform() - parent->GetWorldTransform());
			}
		}
		else
		{
			SetWorldTransformDirty();
		}

		// Place "this" inside the already known shared_ptr so ownership can be transferred
		std::shared_ptr<GameObject> thisPtr{};
		if (m_Parent)

		{
			thisPtr = m_Parent->GetChildSharedPtr(this);
		}
		else
		{
			auto scene{ SceneManager::GetInstance().GetCurrentScene() };
			thisPtr = scene->GetChildSharedPtr(this);
		}

		// Parent's child Handling
		if (m_Parent)
		{
			m_Parent->RemoveChild(this);
		}
		else
		{
			auto scene{ SceneManager::GetInstance().GetCurrentScene() };
			scene->DettatchFromRoot(this);
		}


		m_Parent = parent;

		if (m_Parent)
		{
			m_Parent->AddChild(thisPtr);
		}
		// Attatch to root if nullptr was given as new parent
		else
		{
			auto scene{ SceneManager::GetInstance().GetCurrentScene() };
			scene->AttatchToRoot(thisPtr);
		}
	}

	std::shared_ptr<GameObject> GameObject::GetChildSharedPtr(GameObject* child) const
	{
		for (auto& currentChild : m_Children)
		{
			if (currentChild.get() == child)
			{
				return currentChild;
			}
		}
		return nullptr;
	}

	void GameObject::RemoveChild(GameObject* child)
	{
		m_Children.erase(std::find_if(m_Children.begin(), m_Children.end(), [&](const std::shared_ptr<GameObject>& sp)
			{
				return sp.get() == child;
			}));
	}

	void GameObject::SetLocalTransform(Transform transform)
	{
		m_LocalTransform = transform;
		SetWorldTransformDirty();
	}

	void GameObject::AddLocalTransform(Transform transform)
	{
		m_LocalTransform += transform;
		SetWorldTransformDirty();
	}

	void GameObject::SetWorldTransformDirty()
	{
		m_IsTransformDirty = true;
		for (auto& child : m_Children)
		{
			child->SetWorldTransformDirty();
		}
	}

	Transform GameObject::GetWorldTransform()
	{
		if (m_IsTransformDirty)
		{
			UpdateWorldTransform();
		}
		return m_WorldTransform;
	}

	void GameObject::AddChild(std::shared_ptr<GameObject> child)
	{
		m_Children.emplace_back(child);
	}

	void GameObject::UpdateWorldTransform()
	{
		if (m_IsTransformDirty)
		{
			if (m_Parent != nullptr)
			{
				m_WorldTransform = m_Parent->GetWorldTransform() + m_LocalTransform;
			}
			else
			{
				m_WorldTransform = m_LocalTransform;
			}
			m_IsTransformDirty = false;
		}
	}

	void GameObject::Delete()
	{
		m_IsDeleted = true;
		SetParent(nullptr);
	}

	bool GameObject::IsChild(GameObject* gameObject)
	{
		if (gameObject == nullptr) return false;

		for (auto& child : m_Children)
		{
			if (child.get() == gameObject)
				return true;
		}
		return false;
	}

}
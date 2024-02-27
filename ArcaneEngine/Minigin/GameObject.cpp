#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

GameObject::~GameObject() = default;


bool GameObject::AddComponent(std::shared_ptr<BaseComponent> component)
{
	if (component.get())
	{
		m_Components.emplace_back(component);
		return true;
	}
	return false;
}
	

void GameObject::Initialize()
{
	for (auto& component : m_Components)
	{
		component->Initialize();
	}
	for (auto& child : m_Children)
	{
		child->Initialize();
	}
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
	
	// Parent's child Handling
	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
	}
	m_Parent = parent;
	if (m_Parent)
	{
		m_Parent->AddChild(this);
	}
	// Attatch to root if nullptr was given as new parent
	else
	{
		auto root{ SceneManager::GetInstance().GetCurrentScene()->GetRoot()};
		m_Parent = root.get();
		m_Parent->AddChild(this);
	}
}

void GameObject::RemoveChild(GameObject* child)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
}

void GameObject::SetLocalTransform(Transform transform)
{
	m_LocalTransform = transform;
	SetWorldTransformDirty();
}

void GameObject::SetWorldTransformDirty()
{
	m_WorldTransformIsDirty = true;
	for (auto& child : m_Children)
	{
		child->SetWorldTransformDirty();
	}
}

Transform GameObject::GetWorldTransform()
{
	if (m_WorldTransformIsDirty)
	{
		UpdateWorldTransform();
	}
	return m_WorldTransform;
}

void GameObject::UpdateWorldTransform()
{
	if (m_WorldTransformIsDirty)
	{
		if (m_Parent != nullptr)
		{
			m_WorldTransform = m_Parent->GetWorldTransform() + m_LocalTransform;
		}
		else
		{
			m_WorldTransform = m_LocalTransform;
		}
		m_WorldTransformIsDirty = false;
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
		if (child == gameObject)
			return true;
	}
	return false;
}


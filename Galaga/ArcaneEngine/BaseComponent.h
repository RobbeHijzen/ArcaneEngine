#pragma once
#include <memory>
#include "Transform.h"

namespace AE
{
	class GameObject;

	class BaseComponent
	{
	public:

		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void GameStart() {};

		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void LateUpdate() {};
		virtual void Render() const {};

		Transform GetLocalTransform() const { return m_LocalTransform; }
		Transform GetWorldTransform() const;

		void SetLocalPosition(float x, float y, float z = 0.f) { m_LocalTransform.SetPosition(x, y, z); }
		void AddLocalPosition(float x, float y, float z = 0.f) { m_LocalTransform += {x, y, z}; }

	protected:

		BaseComponent(GameObject* pParent);

		GameObject* GetOwner() const { return m_Owner; }
		Transform m_LocalTransform{};

	private:

		GameObject* m_Owner;
	};
}


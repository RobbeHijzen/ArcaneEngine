#pragma once


namespace AE
{
	class GameObject;

	class FSMState
	{
	public:
		virtual ~FSMState() = default;

		virtual void OnEnter(GameObject* gameObject) = 0;
		virtual void OnExit(GameObject* gameObject) = 0;
		virtual FSMState* Update(GameObject* gameObject) = 0;

	};
}


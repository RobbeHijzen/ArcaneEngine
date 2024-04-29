#pragma once
#include "ArcaneEngine.h"

#include "TextComponent.h"

namespace ObserverPattern
{

	class PrintObserver : public Observer
	{
	public:

		virtual void OnNotify(Event event, GameObject* gameObject) override;
	};

	class HealthDisplayObserver : public Observer
	{
	public:

		HealthDisplayObserver(TextComponent* textComp)
			: m_pTextComponent{ textComp }
		{}

		virtual void OnNotify(Event event, GameObject* gameObject) override;

	private:

		TextComponent* m_pTextComponent{};

	};

	class PickupObserver : public Observer
	{
	public:

		virtual void OnNotify(Event event, GameObject* gameObject) override;

	private:

		int SilverScoreValue{10};
		int GoldScoreValue{100};
	};
	class ScoreDisplayObserver : public Observer
	{
	public:

		ScoreDisplayObserver(TextComponent* textComp)
			: m_pTextComponent{ textComp }
		{}

		virtual void OnNotify(Event event, GameObject* gameObject) override;

	private:

		TextComponent* m_pTextComponent{};

	};

	class BulletObserver : public Observer
	{
	public:

		virtual void OnNotify(Event event, GameObject* gameObject) override;

	private:

	};

}

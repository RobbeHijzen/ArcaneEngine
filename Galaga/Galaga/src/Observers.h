#pragma once
#include "ArcaneEngine.h"

#include "TextComponent.h"

class PrintObserver : public AE::Observer
{
public:

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;
};

class HealthDisplayObserver : public AE::Observer
{
public:

	HealthDisplayObserver(TextComponent* textComp)
		: m_pTextComponent{ textComp }
	{}

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	TextComponent* m_pTextComponent{};

};

class PickupObserver : public AE::Observer
{
public:

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	int SilverScoreValue{10};
	int GoldScoreValue{100};
};
class ScoreDisplayObserver : public AE::Observer
{
public:

	ScoreDisplayObserver(TextComponent* textComp)
		: m_pTextComponent{ textComp }
	{}

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	TextComponent* m_pTextComponent{};

};

class BulletObserver : public AE::Observer
{
public:
	BulletObserver();

	virtual void OnNotify(AE::Event event, AE::GameObject* gameObject) override;

private:

	int m_ShotSoundID{-1};

};



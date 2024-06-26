#pragma once

namespace AE
{
	enum class Event
	{
		GameStart,
		ScoreChanged,
		ObjectDestroyed,

		ObjectLostHealth,
		ObjectDied,

		SilverPickup,
		GoldPickup,

		FireBullet,
		OnOverlap,

		StateChanged,

		LevelChanged,
		WaveStarted,

		ButtonPressed,
		ButtonSelected,

		BossGalagaSpawned,
		AllBossGalagasDied,
		ControlledBossGalagaDied
	};

}

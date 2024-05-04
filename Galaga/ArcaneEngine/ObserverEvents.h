#pragma once

namespace AE
{
	enum class Event
	{
		GameStart,
		ScoreChanged,

		ObjectLostHealth,
		ObjectDied,

		SilverPickup,
		GoldPickup,

		FireBullet,
		OnOverlap
	};

}

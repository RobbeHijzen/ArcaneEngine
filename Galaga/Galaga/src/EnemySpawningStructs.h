#pragma once
#include <glm/glm.hpp>

enum class EnemySeekTypes
{
	Straight,
	Circle
};

struct EnemySeekInfo
{
	EnemySeekTypes seekType;
	glm::vec2 seekPos;
};
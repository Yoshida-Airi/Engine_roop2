#pragma once

#include<cstdint>

enum class CollisionTypeDef : uint32_t
{
	kDefault,
	kPlayer,
	kEnemy,
	
};

enum class ColliderType : uint32_t
{
	SPHERE,
	AABB,
};
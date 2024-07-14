#pragma once

#include<cstdint>

enum class CollisionTypeDef : uint32_t
{
	kDefault,
	kPlayer,
	kWeapon,
	kEnemy,
	kMap,
	
};

enum class ColliderType : uint32_t
{
	SPHERE,
	AABB,
};
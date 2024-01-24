#pragma once

#include<cstdint>

//プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;

//敵陣営
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;

//敵陣営：弾
const uint32_t kCollisionAttributeEnemyBullet = 0b1 << 2;
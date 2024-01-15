#include"Player.h"

void Player::Initialize(ModelData playerData)
{
	player = player->Create(playerData);
}

void Player::Update()
{
	player->Update();
}

void Player::Draw(ICamera* camera)
{
	player->Draw(camera);
}

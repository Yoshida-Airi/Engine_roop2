#include"MyGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MyGame* game = new MyGame();
	game->Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (true)
	{
		game->Update();
		//終了リクエストが来たら抜ける
		if (game->IsEndReqest() == true)
		{
			break;
		}
		game->Draw();

}
	delete game;
	return 0;
}
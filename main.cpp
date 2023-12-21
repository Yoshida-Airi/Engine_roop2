
#include"MyGame.h"
#include"Framework.h"
//Windowsアプリでのエントリーポイント(main関数)
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Framework* game = new MyGame();
	game->Run();

	delete game;

	return 0;
}
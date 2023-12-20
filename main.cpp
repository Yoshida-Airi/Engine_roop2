#include"MyGame.h"
#include"Framework.h"
//Windowsアプリでのエントリーポイント(main関数)
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MyGame* game = new MyGame();
	game->Run();
	delete game;
	return 0;
}
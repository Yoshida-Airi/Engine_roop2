#pragma once
#include<Windows.h>
#include<cstdint>

class WinApp
{
public:

	~WinApp();

	void Initialize();

	//void Update();

	//void Draw();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns></returns>
	bool ProcessMessage();


	/*=======　　　ゲッター	=======*/
	HWND GetHend()const { return hwnd_; };
	HINSTANCE GetGetHInstance()const { return wc_.hInstance; };

private:

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウを表す識別子</param>
	/// <param name="msg">メッセージ</param>
	/// <param name="wparam">ウィンドウメッセージのパラメータ</param>
	/// <param name="lparam">ウィンドウメッセージのパラメータ</param>
	/// <returns></returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// ウィンドウの生成
	/// </summary>
	/// <returns>生成できたかどうか true : できた</returns>
	void CreateGameWindow();

private:

	//クライアント領域のサイズ
	static const int32_t kCilentWidth = 1280;
	static const int32_t kCilentHeight = 720;

	HWND hwnd_ = nullptr;	//ウィンドウを表す識別子
	WNDCLASS wc_{};			//ウィンドウクラスの設定
};


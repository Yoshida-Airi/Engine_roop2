#include "Input.h"

Input* Input::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Input;
	}
	return instance;
}

void Input::Initialize()
{
	winApp_ = WinApp::GetInstance();

	//DirectInputの初期化

	HRESULT hr = DirectInput8Create(winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));

	//キーボードデバイスの生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(hr));

	//入力データ形式のセット
	hr = keyboard->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	assert(SUCCEEDED(hr));

	//排他制御レベルのセット
	hr = keyboard->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
}

void Input::Update()
{
	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	//キーボード情報の取得開始
	keyboard->Acquire();

	//全キーの入力状態を取得する

	keyboard->GetDeviceState(sizeof(key), key);

}

bool Input::PushKey(BYTE keyNumber)
{
	//指定キーを押していればtrueを返す
	if (key[keyNumber])
	{
		return true;
	}

	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (keyPre[keyNumber] != 0 && key[keyNumber] == 0)
	{
#ifdef _DEBUG
		OutputDebugStringA("hit 0!\n");
#endif // _DEBUG

		return true;
	}

	return false;
}

//bool Input::GetJoystickState(int32_t stickNo, XINPUT_STATE& state)
//{
//	DWORD Result = XInputGetState(stickNo, &state);
//	return Result == ERROR_SUCCESS;
//}

Input* Input::instance = NULL;

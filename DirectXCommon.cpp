#include "DirectXCommon.h"


/*======================================*/
/* 　　　　   パブリックメソッド　　　 　     */
/*======================================*/

void DirectXCommon::Initialize()
{
	//DXGIFactoryの初期化
	SetupDXGIFactory();
	//使用する最適なGPUを選択
	SelectOptimalGPU();
	//デバイスの生成
	SetupDevice();
}

void DirectXCommon::Update()
{
}

void DirectXCommon::Draw()
{
}
/*======================================*/
/* 　　　　   プライベートメソッド　　　   　 */
/*======================================*/

/*DXGIファクトリーの生成*/
void DirectXCommon::SetupDXGIFactory()
{
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(hr));
}

/*使用するアダプタ(GPU)を決定する*/
void DirectXCommon::SelectOptimalGPU()
{
	//良い順にアダプタを組む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		//アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));//取得できなかったらエラー
		//ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//採用したアダプタの情報をログに出力
			Log(ConvertString(std::format(L"Use Adapter : {}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;	//ソフトウェアアダプタの場合は見なかったことにする
	}
	//適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);
}

/*デバイスの生成*/
void DirectXCommon::SetupDevice()
{


	ID3D12Device* device = nullptr;
	//機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0 };
	const char* featureLevelString[] = { "12.2","12.1","12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		//採用したアダプタでデバイスを生成
		hr = D3D12CreateDevice(useAdapter, featureLevels[i], IID_PPV_ARGS(&device));

		//採用したアダプタでデバイスが生成できたかを確認
		if (SUCCEEDED(hr))
		{
			//生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelString[i]));
			break;
		}
	}
	//デバイスの生成がうまく行かなかったので起動できない
	assert(device != nullptr);
	Log("Complete create D3D12Device!!!\n");	//初期化完了のログを出す

}


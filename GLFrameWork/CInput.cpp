#include "CInput.h"

LPDIRECTINPUT8 CInput::DInput=nullptr;

CInput::CInput()
{
	pInputDevice = nullptr;
}
HRESULT CInput::Init(HINSTANCE hInstance,HWND hWnd)
{
	HRESULT hr;

	// DirectInputオブジェクトの作成
	if(DInput == NULL)
	{
		hr = DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&DInput,NULL);

		if(FAILED(hr))
		{
			MessageBox(NULL,"インプットオブジェクトの作成に失敗しました","ERROR!",(MB_ICONERROR|MB_OK));
			return hr;
		}
	}
	

	return S_OK;

}

void CInput::Uninit(void)
{
	if(DInput!=nullptr)
	{
		DInput->Release();
		DInput = nullptr;
	}

	if(pInputDevice!=nullptr)
	{
		pInputDevice->Release();
		pInputDevice = nullptr;
	}
}
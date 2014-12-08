#include "CInput.h"

LPDIRECTINPUT8 CInput::DInput=NULL;

CInput::CInput()
{
	pInputDevice = NULL;
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
	if(DInput!=NULL)
	{
		DInput->Release();
		DInput = NULL;
	}

	if(pInputDevice!=NULL)
	{
		pInputDevice->Release();
		pInputDevice = NULL;
	}
}
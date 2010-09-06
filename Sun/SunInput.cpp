// SunInput.cpp: SunInput クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SunUtility.h"
#include "SunFile.h"
#include "SunLog.h"
#include "SunConfig.h"
#include "SunExeption.h"
#include "SunInput.h"
#include "SunApp.h"

CSunInput SunInput;

//////////////////////////////////////////////////////////////////////
// キーボード
//////////////////////////////////////////////////////////////////////

SunKeyboard::SunKeyboard()
{
	m_pdidKeyboard = NULL;
	this->ClearInput();
	m_button[0] = DIK_C;
	m_button[1] = DIK_X;
	m_button[2] = DIK_Z;
	m_button[3] = DIK_LSHIFT;
	m_button[4] = DIK_A;
	m_button[5] = DIK_S;
	m_button[6] = DIK_LCONTROL;
	m_button[7] = DIK_D;
}

SunKeyboard::~SunKeyboard()
{
	this->Release();
}
int SunKeyboard::Release()
{
	if(m_pdidKeyboard != NULL)
	{
		m_pdidKeyboard->Unacquire();
		SAFE_RELEASE(m_pdidKeyboard);
	}
	return 1;
}
//!更新
int SunKeyboard::UpdateInput()
{
	HRESULT hr;
	//取得前に１フレ前の状態保存
	for(int i=0;i<256;++i)
		m_bKeyStateOld[i] = m_bKeyState[i];
	//キーボード
	if( FAILED( hr = m_pdidKeyboard->GetDeviceState(256,(LPVOID)&m_bKeyState)))
	{
		if( hr == DIERR_INPUTLOST ) 
		{
			if(FAILED(hr = m_pdidKeyboard->Acquire()))
				throw SunExeption("キーボードの状態取得に失敗しました\n");
		}
		else
			throw SunExeption("キーボードの状態取得に失敗しました\n");
	}
	//取得
	if(FAILED(m_pdidKeyboard->Acquire()))
	{
		throw SunExeption("キーボードデバイスの取得に失敗しました\n");
		return 0;
	}
	return 1;
}
//!状態取得
int SunKeyboard::GetInputState(int inp)
{
	switch(inp)
	{
	case INP_RIGHT:
		return (m_bKeyState[DIK_RIGHT] & 0x80) !=0;
		break;
	case INP_LEFT:
		return (m_bKeyState[DIK_LEFT] & 0x80) !=0;
		break;
	case INP_DOWN:
		return (m_bKeyState[DIK_DOWN] & 0x80) !=0;
		break;
	case INP_UP:
		return (m_bKeyState[DIK_UP] & 0x80) !=0;
		break;
	default:
		//ボタンの場合
		if(inp>INP_MAX)
			throw SunExeption("ボタンの最大数を超えています\n");
		return (m_bKeyState[m_button[inp-4]] & 0x80) !=0;
		break;
	}
	return 0;
}
//!変化取得
int SunKeyboard::GetInputEvent(int inp)
{
	switch(inp)
	{
	case INP_RIGHT:
		return (m_bKeyState[DIK_RIGHT] & 0x80) !=0 && (m_bKeyStateOld[DIK_RIGHT] & 0x80) ==0;
		break;
	case INP_LEFT:
		return (m_bKeyState[DIK_LEFT] & 0x80) !=0 && (m_bKeyStateOld[DIK_LEFT] & 0x80) ==0;
		break;
	case INP_DOWN:
		return (m_bKeyState[DIK_DOWN] & 0x80) !=0 && (m_bKeyStateOld[DIK_DOWN] & 0x80) ==0;
		break;
	case INP_UP:
		return (m_bKeyState[DIK_UP] & 0x80) !=0 && (m_bKeyStateOld[DIK_UP] & 0x80) ==0;
		break;
	default:
		//ボタンの場合
		if(inp>INP_MAX)
			throw SunExeption("ボタンの最大数を超えています\n");
		return (m_bKeyState[m_button[inp-4]] & 0x80) !=0 && (m_bKeyStateOld[m_button[inp-4]] & 0x80) ==0;
		break;
	}
	return 0;
}
//!クリア
int SunKeyboard::ClearInput()
{
	::ZeroMemory(m_bKeyState,sizeof(m_bKeyState));
	for(int i=0;i<numof(m_bKeyStateOld);++i)
		m_bKeyStateOld[i] = 0x80;
	return 1;
}
//!初期化
int SunKeyboard::Init(LPDIRECTINPUT8 pDI)
{
	// デバイスを取得する。
	if(FAILED(pDI->CreateDevice(GUID_SysKeyboard,&m_pdidKeyboard, NULL)))
		throw SunExeption("キーボードの初期化に失敗しました。\n");
	// データ形式をセットする。
	m_pdidKeyboard->SetDataFormat(&c_dfDIKeyboard);
	// 協調レベルを設定する。
	if(FAILED(m_pdidKeyboard->SetCooperativeLevel(SunApp.m_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		throw SunExeption("キーボードに協調レベルを設定できません\n");
	//アクセス権を取得する。
	if(FAILED(m_pdidKeyboard->Acquire()))
		throw SunExeption("キーボードアクセス権の取得に失敗しました。\n");
	//ボタンコンフィグを読み込み
	char section[MAX_PATH];
	sprintf(section,"Keyboard");
	for(int i=0;i<INP_MAX-4;++i)
	{
		char key[MAX_PATH];
		sprintf(key,"Button%d",i);
		m_button[i] = SunConfig.GetInt2(section,key,m_button[i]);
	}
	return 1;
}
//!生データ取得
int SunKeyboard::GetButtonData()
{
	for(int i=0;i<255;++i)
	{
		if(i!=DIK_ESCAPE && i!=DIK_UP && i!=DIK_DOWN && i!=DIK_LEFT && i!=DIK_RIGHT)
			if(m_bKeyState[i] & 0x80)
				return i;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////
//マウス
//////////////////////////////////////////////////////////////////////

SunMouse::SunMouse()
{
	m_pdidMouse = NULL;
	::ZeroMemory(&m_dims,sizeof(m_dims));
	::ZeroMemory(&m_dimsOld,sizeof(m_dimsOld));
	m_x=SunApp.m_width/2;
	m_y=SunApp.m_height/2;
	m_z=0;
}
SunMouse::~SunMouse()
{
	this->Release();
}
int SunMouse::Release()
{
	SAFE_RELEASE(m_pdidMouse);
	return 1;
}
int SunMouse::Init(LPDIRECTINPUT8 pdi)
{
	//マウスカーソルをウィンドウ内に移動
	{
		RECT rect;
		::GetWindowRect(SunApp.m_hWnd,&rect);
		MoveMouse(rect.left + m_x, rect.right + m_y);
	}
	//デバイスを取得する。
	if(FAILED(pdi->CreateDevice(GUID_SysMouse, &m_pdidMouse, NULL)))
		throw SunExeption("マウスの初期化に失敗しました。\n");
	// データ形式をセットする。
	if(FAILED(m_pdidMouse->SetDataFormat(&c_dfDIMouse2)))
		throw SunExeption("マウスの設定に失敗しました。\n");
	// 協調レベルを設定する。
	if(FAILED(m_pdidMouse->SetCooperativeLevel(SunApp.m_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		throw SunExeption("マウスに協調レベルを設定できません\n");
	//バッファリングデータのサイズ
	#define SAMPLE_BUFFER_SIZE  16
	DIPROPDWORD dipdw;
		// the header
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		// the data
		dipdw.dwData            = SAMPLE_BUFFER_SIZE;
	//プロパティの設定
	if(FAILED(m_pdidMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		throw SunExeption("マウスプロパティを設定できません\n");
	//アクセス権を取得する。
//	if(FAILED(m_pdidMouse->Acquire()))
//		throw SunExeption("マウスのアクセス権の取得に失敗しました。\n");
	return 1;
}
//!更新
int SunMouse::UpdateInput()
{
	HRESULT hr;
	//１フレ前の値を保存
	m_dimsOld = m_dims;
    // Get the input's device state, and put the state in dims
    ZeroMemory( &m_dims, sizeof(m_dims) );
    hr = m_pdidMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_dims );
    if( FAILED(hr) ) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hr = m_pdidMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = m_pdidMouse->Acquire();

        // Update the dialog text 
        if( hr == DIERR_OTHERAPPHASPRIO || 
            hr == DIERR_NOTACQUIRED ) 
		{
			SunLog("Mouse is unaquired.\n");
		}
    }
//	SunApp.Output("x:%d y:%d\n",m_x,m_y);
	m_x += 1.5*m_dims.lX;
	m_y += 1.5*m_dims.lY;
	m_z += 1.5*m_dims.lZ;
	m_x = max(0,min(SunApp.m_width,m_x));
	m_y = max(0,min(SunApp.m_height,m_y));
	if(m_x < 0 || m_x > SunApp.m_width || m_y < 0 || m_y > SunApp.m_height)
	{
		SunApp.MoveMouse(m_x, m_y);
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////
// ジョイスティック
//////////////////////////////////////////////////////////////////////

SunJoystick::SunJoystick()
{
	m_pdidJoystick = NULL;
	this->ClearInput();
	m_name[0]='\0';
	for(int i=0;i<INP_MAX-4;++i)
		m_button[i] = i;
	m_asobi = 0;
}

SunJoystick::~SunJoystick()
{
	this->Release();
}
int SunJoystick::Release()
{
	if(m_pdidJoystick != NULL)
	{
		m_pdidJoystick->Unacquire();
		SAFE_RELEASE(m_pdidJoystick);
	}
	return 1;
}
//!初期化
int SunJoystick::Init(CSunInput* pInput,const DIDEVICEINSTANCE* pdidInstance)
{
	SunLog("JoyStick [ %d ]\n",pInput->m_lstpJoystick.size());
	SunLog("	Instance[ %s ]\n",&pdidInstance->tszInstanceName);
	SunLog("	Product[ %s ]\n",&pdidInstance->tszProductName);
	//デバイス名を保存
	strcpy(m_name,pdidInstance->tszProductName);
	//ジョイスティック番号を保存
	this->m_number = pInput->m_lstpJoystick.size();
	// デバイスを生成する。
	if(FAILED(pInput->m_pDI->CreateDevice(pdidInstance->guidInstance,&m_pdidJoystick, NULL)))
		throw SunExeption("ジョイスティックを初期化できません\n");
	//データフォーマット
	if(FAILED(m_pdidJoystick->SetDataFormat( &c_dfDIJoystick2 )))
		throw SunExeption("ジョイスティックを初期化できません\n");
	//協調レベル
	if(FAILED(m_pdidJoystick->SetCooperativeLevel( SunApp.m_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		throw SunExeption("ジョイスティックに協調レベルを設定できません\n");
	//軸の範囲の設定
	if(FAILED(m_pdidJoystick->EnumObjects( SunJoystick::EnumObjectsCallback, (VOID*)this, DIDFT_ALL )))
		throw SunExeption("ジョイスティックの初期化に失敗しました。\n");
	//ボタンコンフィグを読み込み
	char section[MAX_PATH];
	sprintf(section,"Joystick%d",m_number);
	for(int i=0;i<INP_MAX-4;++i)
	{
		char key[MAX_PATH];
		sprintf(key,"Button%d",i);
		m_button[i] = SunConfig.GetInt2(section,key,i);
	}
	m_asobi = SunConfig.GetInt2(section,"asobi",600);
	return 1;
}
//!更新
int SunJoystick::UpdateInput()
{
	HRESULT hr;
	//取得前に１フレ前の値として保存
	m_diJoyStateOld = m_diJoyState;
	//ジョイスティック
	if(m_pdidJoystick)
	{
		if(FAILED(m_pdidJoystick->Acquire()))
			throw SunExeption("ジョイスティックデバイスのアクセス権取得ができません\n");
		if( FAILED( m_pdidJoystick->Poll()))
		{
			hr = m_pdidJoystick->Acquire();
			//for(int i=0; i>10 || hr == DIERR_INPUTLOST ;++i) 
			while( hr == DIERR_INPUTLOST)
				hr = m_pdidJoystick->Acquire();
		}
		if(FAILED(m_pdidJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &m_diJoyState )))
			throw SunExeption("ジョイスティックデバイス状態の取得に失敗しました\n");
	}
	return 1;
}
//!状態取得
int SunJoystick::GetInputState(int inp)
{
	switch(inp)
	{
	case INP_RIGHT:
		return m_diJoyState.lX > m_asobi;
		break;
	case INP_LEFT:
		return m_diJoyState.lX < -m_asobi;
		break;
	case INP_DOWN:
		return m_diJoyState.lY > m_asobi;
		break;
	case INP_UP:
		return m_diJoyState.lY < -m_asobi;
		break;
	default:
		//ボタンの場合
		if(inp > INP_MAX)
			throw SunExeption("ボタンの最大数を超えています\n");
		return (m_diJoyState.rgbButtons[m_button[inp-4]] & 0x80) !=0;
	}
	return 0;
}
//!変化取得
int SunJoystick::GetInputEvent(int inp)
{
	switch(inp)
	{
	case INP_RIGHT:
		return m_diJoyState.lX > m_asobi && m_diJoyStateOld.lX <= m_asobi;
		break;
	case INP_LEFT:
		return m_diJoyState.lX < -m_asobi && m_diJoyStateOld.lX >= -m_asobi;
		break;
	case INP_DOWN:
		return m_diJoyState.lY > m_asobi && m_diJoyStateOld.lY <= m_asobi;
		break;
	case INP_UP:
		return m_diJoyState.lY < -m_asobi && m_diJoyStateOld.lY >= -m_asobi;
		break;
	default:
		//ボタンの場合
		if(inp>INP_MAX)
			throw SunExeption("ボタンの最大数を超えています\n");
		return (m_diJoyState.rgbButtons[m_button[inp-4]] & 0x80) !=0 && (m_diJoyStateOld.rgbButtons[m_button[inp-4]] & 0x80) ==0;
		break;
	}
	return 0;
}
//!クリア
int SunJoystick::ClearInput()
{
	::ZeroMemory(&m_diJoyState,sizeof(m_diJoyState));
	::ZeroMemory(&m_diJoyStateOld,sizeof(m_diJoyStateOld));
	for(int i=0; i<INP_MAX; ++i)
		m_diJoyStateOld.rgbButtons[i] = 0x80;
	return 1;
}
//!生データ取得
int SunJoystick::GetButtonData()
{
	//rgbButtonsは配列数が１２８なので・・（笑）
	for(int i=0;i<128;++i)
	{
		if(m_diJoyState.rgbButtons[i])
			return i;
	}
	//ボタンが押されていなければ-1を返します。
	return -1;
}
//値の範囲の設定
BOOL CALLBACK SunJoystick::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
	SunJoystick *pInput = (SunJoystick*)pContext;

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        // Set the range for the axis
        if( FAILED(pInput->m_pdidJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
			throw SunExeption("ジョイスティックの初期化に失敗しました。\n");
         
    }
    return DIENUM_CONTINUE;
}
//////////////////////////////////////////////////////////////////////
// インプット総括
//////////////////////////////////////////////////////////////////////

CSunInput::CSunInput()
{
	m_pDI = NULL;
	::ZeroMemory(m_state,sizeof(m_state));
	::ZeroMemory(m_device,sizeof(m_device));
	m_usekeyboard = 1;
	m_usemouse = 1;
	m_usejoystick = 1;
}

CSunInput::~CSunInput()
{
	this->Release();
}
int CSunInput::Release()
{
	try
	{
		for(SunJoyItr i=m_lstpJoystick.begin();i!=m_lstpJoystick.end();++i)
			SAFE_DELETE(*i)
		m_lstpJoystick.clear();
		m_keyboard.Release();
		m_mouse.Release();
		SAFE_RELEASE(m_pDI);
	}catch(...){MsgBox("DirectInputの解放で不明な例外しました。");}
	return 1;
}
//!Input初期化
int CSunInput::InitInput()
{
	// DirectInput オブジェクトを作成する。
	if(FAILED(DirectInput8Create(::GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
		throw SunExeption("DirectInputの初期化に失敗しました。\n");

	//キーボード初期化
	if(m_usekeyboard)
		m_keyboard.Init(m_pDI);

	//マウス初期化
	if(m_usemouse)
		m_mouse.Init(m_pDI);

	//ジョイスティック列挙
	if(m_usejoystick)
		if(FAILED(m_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,(VOID*)this, DIEDFL_ATTACHEDONLY)))
			throw SunExeption("ジョイスティックの初期化に失敗しました。\n");

	//コンフィグ
	for(int i=0;i<PLAYER_NUM;++i)
	{
		//コンフィグのデフォルトを作る
		if(i<m_lstpJoystick.size())
			m_device[i] = i;	//ジョイスティック
		else
			m_device[i] = -1;	//キーボード
		
		//コンフィグ読み込み
		char buffer[16];
		sprintf(buffer,"player%d",i);
		m_device[i] = SunConfig.GetInt2("keyconfig",buffer,m_device[i]);
	}
	return 1;
}
//!全デバイスを更新
int CSunInput::UpdateInput()
{
	//更新前に１フレ前の値を取得
	for(int i=0;i<INP_MAX;++i)
		m_state[i] = this->GetInputState(i);
	//キーボード更新
	if(m_usekeyboard)
		if(!m_keyboard.UpdateInput())
			return 0;
	//マウス更新
	if(m_usemouse)
		if(!m_mouse.UpdateInput())
			return 0;
	//全ジョイスティック更新
	if(m_usejoystick)
		for(SunJoyItr i=m_lstpJoystick.begin();i!=m_lstpJoystick.end();++i)
			if(!(*i)->UpdateInput())
				return 0;
	return 1;
}
//!全デバイスの状態から取得
int CSunInput::GetInputState(int inp)
{
	if(m_keyboard.GetInputState(inp))
		return 1;
	for(SunJoyItr i=m_lstpJoystick.begin();i!=m_lstpJoystick.end();++i)
		if((*i)->GetInputState(inp))
			return 1;
	return 0;
}
//!押された瞬間か？
int CSunInput::GetInputEvent(int inp)
{
	if(m_state[inp] == 0 && this->GetInputState(inp) == 1)
		return 1;
	return 0;
}
//!離し
int CSunInput::GetInputHanasi(int inp)
{
	if(m_state[inp] == 1 && this->GetInputState(inp) == 0)
		return 1;
	return 0;
}
//!プレイヤー番号のデバイスから取得
int CSunInput::GetInputState2(int player,int inp)
{
	if(m_device[player] == -1) //キーボード
		return m_keyboard.GetInputState(inp);
	else if(m_device[player] >= 0) //ジョイスティック
	{
		SunJoystick* pJoystick = GetJoystick(m_device[player]);
		if(pJoystick!=NULL)
			return pJoystick->GetInputState(inp);
	}
	return 0;
}
//!押された瞬間か？
int CSunInput::GetInputEvent2(int player,int inp)
{
	if(m_device[player] == -1) //キーボード
		return m_keyboard.GetInputEvent(inp);
	else if(m_device[player] >= 0) //ジョイスティック
	{
		SunJoystick* pJoystick = GetJoystick(m_device[player]);
		if(pJoystick!=NULL)
			return pJoystick->GetInputEvent(inp);
	}
	return 0;
}
//!マウス
int CSunInput::GetMouseX()
{
	return m_mouse.m_x;
}
int CSunInput::GetMouseY()
{
	return m_mouse.m_y;
}
int CSunInput::GetMouseZ()
{
	return m_mouse.m_z;
}
void CSunInput::SetMouseXY(int x,int y)
{
	m_mouse.m_x=x;
	m_mouse.m_y=y;
}
//!全デバイスをクリア
int CSunInput::ClearInput()
{
	m_keyboard.ClearInput();
	for(SunJoyItr i=m_lstpJoystick.begin();i!=m_lstpJoystick.end();++i)
		(*i)->ClearInput();
	//押された瞬間にならないよう、すでに押されている状態とみなす
	for(int i=0;i<INP_MAX;++i)
		m_state[i] = 1;
	return 1;
}
//!再起動可能も可能な初期化
int CSunInput::ReInit()
{
	if(!this->Release())
		return 0;
	if(!this->InitInput())
		return 0;
	if(!this->UpdateInput())
		return 0;
	return 1;
}
/**
	生データ取得
	キーコンフィグ用
	ボタンが押されていなければ-1を返す
	方向キーは無視
*/
int CSunInput::GetButtonData()
{
	for( SunJoyItr i = m_lstpJoystick.begin(); i != m_lstpJoystick.end(); ++i ) {
		int inp = (*i)->GetButtonData();
		if( inp >= 0 )
			return inp;
	}
	return -1;
}
/**
	生データ取得 デバイス指定
	キーコンフィグ用
	ボタンが押されていなければ-1を返す
	方向キーは無視
*/
int CSunInput::GetButtonData(int device)
{
	if(device == -1) //キーボード
		return m_keyboard.GetButtonData();
	else if(device >= 0) //ジョイスティック
	{
		SunJoystick* pJoystick = GetJoystick(device);
		if(pJoystick!=NULL)
			return pJoystick->GetButtonData();
	}
	return -1;
}
/**
	生データ取得 複数プレイヤー対応
	キーコンフィグ用
	ボタンが押されていなければ-1を返す
	方向キーは無視
*/
int CSunInput::GetButtonData2(int player)
{
	if(m_device[player] == -1) //キーボード
		return m_keyboard.GetButtonData();
	else if(m_device[player] >= 0) //ジョイスティック
	{
		SunJoystick* pJoystick = GetJoystick(m_device[player]);
		if(pJoystick!=NULL)
			return pJoystick->GetButtonData();
	}
	return -1;
}
//DIKから取得
int CSunInput::GetKeyState(int key) {
	return m_keyboard.m_bKeyState[key] & 0x80;
}
int CSunInput::GetKeyEvent(int key) {
	return (m_keyboard.m_bKeyState[key] & 0x80) &&
		 (m_keyboard.m_bKeyStateOld[key] & 0x80);
}

//擬似list::operator[]
SunJoystick* CSunInput::GetJoystick(int index)
{
	int i=0;
	for(SunJoyItr itr=m_lstpJoystick.begin();itr!=m_lstpJoystick.end();++itr)
	{
		if(index == i)
			return *itr;
		++i;
	}
	return NULL;
}
//!InputコールバックStatic関数
BOOL CALLBACK CSunInput::EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
{
	CSunInput *pInput = (CSunInput*)pContext;
	SunJoystick* pJoystick = new SunJoystick;
	//ジョイスティック初期化
	if(!pJoystick->Init(pInput, pdidInstance))
	{
		SunLog("ジョイスティックの初期化に失敗[%s]\n",pJoystick->m_name);
		SAFE_DELETE(pJoystick);
		return DIENUM_CONTINUE;
	}
	//リストに追加
	pInput->m_lstpJoystick.push_back(pJoystick);
    return DIENUM_CONTINUE;
}

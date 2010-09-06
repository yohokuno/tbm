// SunInput.h: SunInput クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

//入力情報
#define INP_NEUTRAL	-1
#define INP_RIGHT	0
#define INP_LEFT	1
#define INP_DOWN	2
#define INP_UP		3
#define INP_B0		4
#define INP_B1		5
#define INP_B2		6
#define INP_B3		7
#define INP_B4		8
#define INP_B5		9
#define INP_B6		10
#define INP_B7		11
#define INP_MAX		12

//プレーヤー数（キーコンフィグの数に影響）
#define PLAYER_NUM 1

/**
	@brief キーボード入力
*/
class SunKeyboard
{
	friend class CSunInput;
private:
	LPDIRECTINPUTDEVICE8	m_pdidKeyboard;
	BYTE		m_bKeyState[256];		///< キーボード状態
	BYTE		m_bKeyStateOld[256];	///< キーボード状態（１フレ前）
	int		m_button[INP_MAX-4];		///< ボタンコンフィグ（方向キーの４つを除く）
public:
	SunKeyboard();
	virtual ~SunKeyboard();
	int Release();
	int GetInputState(int inp);	///<
	int GetInputEvent(int inp);	///< マクロ（INP_**)から取得
	int UpdateInput();
	int ClearInput();
	int Init(LPDIRECTINPUT8);
	int GetButtonData();		///< 生データ取得
	int GetKeyboardState();		///< DIKから取得
};
/**
	@brief マウス入力
*/
class SunMouse
{
	friend class CSunInput;
private:
	LPDIRECTINPUTDEVICE8	m_pdidMouse;
	DIMOUSESTATE2 m_dims;		//マウス状態
	DIMOUSESTATE2 m_dimsOld;	//マウス状態（１フレ前）
	float m_x,m_y,m_z;			//マウスポインタの絶対位置
public:
	SunMouse();
	virtual ~SunMouse();
	int Release();
	int UpdateInput();
	int ClearInput();
	int Init(LPDIRECTINPUT8);
	int GetButtonState(int inp){return m_dims.rgbButtons[inp];}
	int GetButtonEvent(int inp){return m_dims.rgbButtons[inp] && !m_dimsOld.rgbButtons[inp];}
};
/**
	@brief ジョイスティック入力
*/
class SunJoystick
{
	friend class CSunInput;
private:
	LPDIRECTINPUTDEVICE8	m_pdidJoystick;
	DIJOYSTATE2		m_diJoyState;		///< ジョイスティック状態
	DIJOYSTATE2		m_diJoyStateOld;	///< ジョイスティック状態(１フレ前）
	TCHAR	m_name[MAX_PATH];			///< 名前
	int		m_number;					///< ジョイスティック番号
	int		m_button[INP_MAX-4];		///< ボタンコンフィグ（方向キーの４つを除く）
	int		m_asobi;					///< キーの遊び
public:
	SunJoystick();
	virtual ~SunJoystick();
	int Release();
	int Init(CSunInput* pInput,const DIDEVICEINSTANCE* pdidInstance);
	int UpdateInput();
	int GetInputState(int inp);	///< マクロ（INP_**)から取得
	int GetInputEvent(int inp);	///< マクロ（INP_**)から取得
	int GetButtonData();		///< 生データ取得
	int ClearInput();
	static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
};
typedef list<SunJoystick*>::iterator SunJoyItr;

/**
	@brief すべての入力
*/
class CSunInput
{
	friend SunJoystick;
public:
	LPDIRECTINPUT8			m_pDI;				///<
	SunKeyboard				m_keyboard;			///<キーボード
	SunMouse				m_mouse;			///<マウス
	list<SunJoystick*>		m_lstpJoystick;		///<ジョイスティックリスト
	int		m_state[INP_MAX];		///<１フレ前の状態
	int		m_device[PLAYER_NUM];	///<１Ｐ，２Ｐの使用デバイス番号（-1:キーボード、0以上：ジョイスティック)
	int		m_usekeyboard;				///< キーボード利用フラグ
	int		m_usemouse;					///< マウス利用フラグ
	int		m_usejoystick;				///< ジョイスティック利用フラグ
public:
	CSunInput();
	virtual ~CSunInput();
	int Release();				///<
	int	InitInput();			///< 使わない。初期化用
	int ReInit();				///< （再）初期化、こちらを使うこと
	int GetInputState(int inp);	///< その瞬間の入力状態
	int GetInputEvent(int inp);	///< 押された瞬間を検出
	int GetInputHanasi(int inp);///< 離しを検出
	int GetInputState2(int player,int inp);	///< 複数プレーヤー対応
	int GetInputEvent2(int player,int inp);	///< 複数プレーヤー対応
	int GetMouseX();			///<
	int GetMouseY();			///<
	int GetMouseZ();			///<
	int GetMouseState(int inp){return m_mouse.GetButtonState(inp);}
	int GetMouseEvent(int inp){return m_mouse.GetButtonEvent(inp);}
	void SetMouseXY(int x,int y);
	void UseKeyboard(int use){m_usekeyboard=use;}
	void UseJoystick(int use){m_usekeyboard=use;}
	void UseMouse(int use){m_usemouse=use;}
	int UpdateInput();			///< 更新
	int ClearInput();			///<
	int GetButtonData();			///< 生データ取得（押されてなければ-1を返す
	int GetButtonData(int device);	///< デバイス指定
	int GetButtonData2(int player);	///< 複数プレーヤー対応
	int GetKeyState(int key);		///< DIK_*から取得
	int GetKeyEvent(int key);		///< DIK_*から取得
	SunJoystick* GetJoystick(int index);	///< 擬似list::operaotr[]
	static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
};
extern CSunInput SunInput;

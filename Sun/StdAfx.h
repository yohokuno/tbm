/**
	@file StdAfx.h
	@brief プリコンパイルヘッダ

	標準のシステム インクルード ファイル、
	または参照回数が多く、かつあまり変更されない
	プロジェクト専用のインクルード ファイルを記述します。

	@todo プログラムで必要なヘッダー参照を追加してください。
**/
#pragma once

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します
#define STRICT

//ＷＩＮ３２ＡＰＩ
#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <winnls32.h>
#include <process.h>
#include <mmsystem.h>
//DirectX8
#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>

#include <dinput.h>
#include <dsound.h>
#include <dmusici.h>

//標準ライブラリ
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <mbstring.h>
#include <tchar.h>

//ＳＴＬ
#include <list>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
using namespace std;
//ＢＯＯＳＴ
//#include <boost/shared_ptr.hpp> 
//using namespace boost;
//ＡＴＬ・ＷＴＬ
#include <atlbase.h>
/*
#define WINVER		0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100
#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>
*/
//メモリリークチェック用
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//ライブラリの明示的リンク
//#pragma comment(lib,"d3d9")
#pragma comment(lib,"d3dx9")
#pragma comment(lib,"dxguid")
#pragma comment(lib,"winmm")
#pragma comment(lib,"dinput8")
#pragma comment(lib,"dsound")

//double->float変換警告を外す
#pragma warning( disable : 4305 )

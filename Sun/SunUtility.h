/**
	@file SunUtility.h
	@brief ユーティリティファイル

	クラスにするまでもないユーティリティ群です。
	stdafx以外のほかのファイルには依存しません。
*/

#pragma once

//!ポインタの安全な操作
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p);	  (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
//!円周率
#define PAI (3.1415926535897932384626)
#define PI	(3.1415926535897932384626)
//!自然対数の底
#define EXP (2.7182818284590452353602)
//!forスコープ閉じ込め用
#define for						if ( false ) ; else for
//!配列の要素数を求める
#define numof(array) (sizeof(array)/sizeof(array[0]))
//!透過色
#define COLOR_TRANS D3DCOLOR_ARGB(255,255,0,255)
//!しろ
#define COLOR_WHITE D3DCOLOR_ARGB(255,255,255,255)

//!2乗
inline double	pow2(double x){return x*x;}
//!底が２の対数
inline double	log2(double x){return log(x)/log(2);}
//!２のるい乗以外切り上げ
inline int		ceil2(int x){return (int)pow(2.0,ceil(log2(x)));}
//!２のるい乗以外切捨て
inline int		floor2(int x){return (int)pow(2.0,floor(log2(x)));}
//!割り算の余り（%と違うのは負数を渡したとき正数になって返ること）
inline int		mod(int a,int b){return a>=0 ? a%b : b+a%b;}
//!三角関数
inline double Cos(double a){return cos(2.0*PI*a/360);}
inline double Sin(double a){return sin(2.0*PI*a/360);}
inline double Tan(double a){return tan(2.0*PI*a/360);}
inline double Rad2Deg(double r){return r*180/PI;}
inline double Deg2Rad(double d){return d*PI/180;}
inline double Atan2(double x,double y){return Rad2Deg(atan2(y,x));}
//シグモイド関数
inline double Sigmoid(double x,double max,double min,double s){return min+(max-min)*exp(x)/(s+exp(x));}
//!アルファつきD3DCOLOR
inline D3DCOLOR	argb(BYTE a,BYTE r, BYTE g,BYTE b){return D3DCOLOR_ARGB(a,r,g,b);}
//!アルファなしD3DCOLOR
inline D3DCOLOR	xrgb(BYTE r, BYTE g,BYTE b){return D3DCOLOR_XRGB(r,g,b);}
//!アルファのみD3DCOLOR
inline D3DCOLOR alpha(BYTE a){return D3DCOLOR_ARGB(a,255,255,255);}
//!モノクロ
inline D3DCOLOR mono(BYTE c){return D3DCOLOR_XRGB(c,c,c);}
//!メッセージボックス
inline int MsgBox(char* pStr,char* pCaption="エラー"){return ::MessageBox(NULL,pStr,pCaption,MB_OK | MB_ICONERROR);}
//!カレントディレクトリ
string GetCurrent();
void ResetCurrent();
//マウス
DWORD   GetMouseParam(int nXY,bool bX);
void	MoveMouse(int nX,int nY);

//矩形同士の当たり判定
int GetHantei(RECT *r1,RECT *r2);

//フォルダ作成（スナップショット・リプレイ用）
int CreateDirectory( char* name );

//整数累乗
int pown( int x, int n );

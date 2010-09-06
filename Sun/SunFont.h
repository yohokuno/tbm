// SunFont.h: SunFont クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SunTexture.h"

struct SunFontTexture {
	SunTexture		m_tex;
	GLYPHMETRICS	m_gm;
};
/**
	フォントクラス

	全角文字１つ＝１テクスチャとして、テクスチャリストに保存します。
	先にフォントを作っておけばただのテクスチャの描画なので、
	パフォーマンスは画像ファイルを使うのと変わらないはず。
	いや、テクスチャが分割されてる分遅いけど（汗）
*/
class SunFont
{
public:
	list<SunFontTexture*>	m_lstpTex;
	string		m_str;			///< 文字列
	TEXTMETRIC	m_tm;			///< 現在のフォント情報
	int			m_interval;		///< 行間
public:
	SunFont();
	SunFont(char* pStr,int fontSize=16,const char* pFontName="ＭＳ ゴシック",int interval = 4);
	virtual ~SunFont();
	int Release();
	int Create(char* pStr,int fontSize=16,const char* pFontName="ＭＳ ゴシック");
	int CreateList(char* pStr,int fontSize=16,const char* pFontName="ＭＳ ゴシック");
	int Draw(int x,int y,D3DCOLOR color=alpha(255),double scale=1,double angle=0);
	int DrawGradation(int x,int y,D3DCOLOR under=D3DCOLOR_XRGB(255,0,0),D3DCOLOR top=alpha(255),double scale=1,double angle=0);
	int DrawFormat(int x,int y,D3DCOLOR color=D3DCOLOR_XRGB(255,0,0),BYTE a=255);
	int DrawFlame(int x,int y,D3DCOLOR c1=D3DCOLOR_XRGB(255,255,255),D3DCOLOR c2=D3DCOLOR_XRGB(0,0,0));

	//アンチエイリアスつきフォント描画関数
	HRESULT CreateTextureFont( SunFontTexture* pTexture, char *str, int w, int h, LPCTSTR font_name, D3DCOLOR color );

};
typedef list<SunFontTexture*>::iterator SunFontItr;


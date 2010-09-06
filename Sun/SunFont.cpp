// SunFont.cpp: SunFont クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunApp.h"
#include "SunConfig.h"
#include "Sun3D.h"
#include "SunSurface.h"
#include "SunTexture.h"
#include "SunExeption.h"
#include "SunSprite.h"
#include "SunFont.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
SunFont::SunFont(){
	m_interval = 5;
}
SunFont::SunFont(char* pStr,int fontSize,const char* pFontName,int interval)
{
	m_interval = interval;
//	if( !Sun3D.m_square )
//		Create(pStr,fontSize,pFontName);
//	else
		CreateList(pStr,fontSize,pFontName);
}

SunFont::~SunFont()
{
	this->Release();
}
/**
	フォントテクスチャ作成
	
	正方形以外のテクスチャが扱えるものとして、文字列を長方形テクスチャに入れます。
	当然2の累乗制限があるので、横幅が512を超えるテクスチャになるとかなり無駄です…
	なお、squareオプションがONのときはさらに縦幅を無駄にして正方形テクスチャを使います。
*/
int SunFont::Create(char* pStr,int fontSize,const char* pFontName)
{
	this->Release();
	const char* p=pStr;				//文字ポインタ
	if( p != NULL && *p != '\0')
	{
		m_str = pStr;

		//新規作成
		SunFontTexture* pTex = new SunFontTexture;

		//テクスチャ初期化
		if(FAILED(CreateTextureFont( pTex, pStr, fontSize, fontSize, pFontName, alpha(255))))
			return 0;

		//リストに追加
		m_lstpTex.push_back(pTex);
	}
	return 1;
}
/**
	フォントテクスチャリスト作成
	
	正方形テクスチャを使って、1文字=1テクスチャとしてテクスチャリストに格納します。
	テクスチャの分割によってどれだけ重くなるかはフォントに限らず興味あるところ…
*/
int SunFont::CreateList(char* pStr,int fontSize,const char* pFontName)
{
	this->Release();
	m_str = pStr;
	const char* p=pStr;				//文字ポインタ
	while( *p != '\0')
	{
		//新規作成
		SunFontTexture* pTex = new SunFontTexture;
		//全角か半角かで場合分け
		BOOL zenkaku = ::IsDBCSLeadByte( *p );
		if(zenkaku)
		{//全角
			char buffer[3];
			buffer[0] = *p;
			buffer[1] = *(p+1);
			buffer[2] = '\0';
			p+=2;	//２バイト進める
			//文字描画
			CreateTextureFont( pTex, buffer, fontSize, fontSize, pFontName, alpha(255));
		}
		else
		{//半角
			char buffer[2];
			buffer[0] = *p;
			buffer[1] = '\0';
			p+=1;	//１バイト進める
			//文字描画
			CreateTextureFont( pTex, buffer, fontSize, fontSize, pFontName, alpha(255));
		}
		//リストに追加
		m_lstpTex.push_back(pTex);
	}
	return 1;
}
/**
	開放
*/
int SunFont::Release()
{
	for(SunFontItr i=m_lstpTex.begin(); i!=m_lstpTex.end(); ++i) {
		SAFE_DELETE(*i);
	}
	m_lstpTex.resize(0);
	m_str.resize(0);
	return 1;
}
/**
	描画
*/
int SunFont::Draw(int x,int y,D3DCOLOR color,double scale,double angle)
{
	double x2 = x, y2 = y;
	double mm = 0;
	const char* p = m_str.c_str();				//文字ポインタ
	for(SunFontItr i = m_lstpTex.begin(); i != m_lstpTex.end(); ++i)
	{
		//とりあえず描画
		if( *i != NULL ) {
			(*i)->m_tex.Draw(x2,y2,color,scale,Deg2Rad(angle));
		}
		//全角か半角かで場合分け
		BOOL zenkaku = ::IsDBCSLeadByte( *p );
		double mx=0,my=0;
		if(zenkaku)
		{//全角
			// 次の文字の描画位置へ移動
			mx = (*i)->m_gm.gmCellIncX;
			my = (*i)->m_gm.gmCellIncY;
			x2 += mx * Cos(angle) + my * Sin(angle);
			y2 += mx * Sin(angle) + my * Cos(angle);
			p+=2;	//２バイト進める
		}
		else
		{//半角
			if(*p == '\n')//エスケープシークエンス
			{
				mx = -mm;
				mm = 0;
				my = m_tm.tmHeight + m_tm.tmExternalLeading + m_tm.tmInternalLeading + m_interval;
				x2 += mx * Cos(angle) + my * Sin(angle);
				y2 += mx * Sin(angle) + my * Cos(angle);
			}
			else{
				// 次の文字の描画位置へ移動
				mx = (*i)->m_gm.gmCellIncX;
				my = (*i)->m_gm.gmCellIncY;
				x2 += mx * Cos(angle) + my * Sin(angle);
				y2 += mx * Sin(angle) + my * Cos(angle);
			}
			p += 1;	//１バイト進める
		}
		mm += mx;
	}
	return 1;
}
/**
	グラデーション
*/
int SunFont::DrawGradation(int x,int y,D3DCOLOR under,D3DCOLOR top,double scale,double angle)
{
	double x2 = x, y2 = y;
	double mm = 0;
	const char* p = m_str.c_str();				//文字ポインタ
	for(SunFontItr i = m_lstpTex.begin(); i != m_lstpTex.end(); ++i)
	{
		//とりあえず描画
		if( *i != NULL ) {
			(*i)->m_tex.DrawGradation(x2,y2,under,top,scale);
		}
		//全角か半角かで場合分け
		BOOL zenkaku = ::IsDBCSLeadByte( *p );
		double mx=0,my=0;
		if(zenkaku)
		{//全角
			// 次の文字の描画位置へ移動
			mx = (*i)->m_gm.gmCellIncX;
			my = (*i)->m_gm.gmCellIncY;
			x2 += mx * Cos(angle) + my * Sin(angle);
			y2 += mx * Sin(angle) + my * Cos(angle);
			p+=2;	//２バイト進める
		}
		else
		{//半角
			if(*p == '\n')//エスケープシークエンス
			{
				mx = -mm;
				mm = 0;
				my = m_tm.tmHeight + m_tm.tmExternalLeading + m_tm.tmInternalLeading + m_interval;
				x2 += mx * Cos(angle) + my * Sin(angle);
				y2 += mx * Sin(angle) + my * Cos(angle);
			}
			else{
				// 次の文字の描画位置へ移動
				mx = (*i)->m_gm.gmCellIncX;
				my = (*i)->m_gm.gmCellIncY;
				x2 += mx * Cos(angle) + my * Sin(angle);
				y2 += mx * Sin(angle) + my * Cos(angle);
			}
			p += 1;	//１バイト進める
		}
		mm += mx;
	}
	return 1;
}
/**
	グラデーション（上は白、下は指定色）＋影つきテキスト描画
*/
int SunFont::DrawFormat(int x,int y,D3DCOLOR color,BYTE a){
	D3DCOLOR under = (color & 0x00FFFFFF) | argb(a,0,0,0);
	this->Draw(x+1,y+2,argb(a,0,0,0),1);
	this->DrawGradation(x,y,under,alpha(a),1);
	return 1;
}
/**
	ふち付き描画
*/
int SunFont::DrawFlame(int x,int y,D3DCOLOR c1,D3DCOLOR c2) {
	this->Draw(x+1,y,c2);
	this->Draw(x-1,y,c2);
	this->Draw(x,y+1,c2);
	this->Draw(x,y-1,c2);
	this->Draw(x+1,y+1,c2);
	this->Draw(x-1,y+1,c2);
	this->Draw(x+1,y-1,c2);
	this->Draw(x-1,y-1,c2);
	this->Draw(x,y,c1);
	return 1;
}
/**
	アンチエイリアスつきフォント描画関数
*/
HRESULT SunFont::CreateTextureFont( SunFontTexture* pTexture, char *str, int w, int h, LPCTSTR font_name ,D3DCOLOR color)
{
	HRESULT hr = S_OK;;

	// デバイスコンテキストの捏造
	HWND hDskWnd = GetDesktopWindow();
	HDC hDskDC = GetDC( hDskWnd );
	HDC hdc = CreateCompatibleDC( hDskDC );

	// フォントの作成
	HFONT hFont;
	hFont = CreateFont(
		h,							// フォントの高さ
		w / 2,						// フォントの横幅？
		0,							// 多分０でよし
		0,							// 多分０でよし
		FW_SEMIBOLD,				// フォントの太さ
		FALSE,						// 斜体かどうか
		FALSE,						// 下線のタイプ
		FALSE,						// 打ち消し線
		SHIFTJIS_CHARSET,			// フォントの文字セット
		OUT_DEFAULT_PRECIS,			// 出力精度
		CLIP_DEFAULT_PRECIS,		// クリッピング
		ANTIALIASED_QUALITY,		// 出力品質
		DEFAULT_PITCH,				// pitch and family
		font_name					// フォントの種類（ NULL でも可 ）
	);
	
	//フォントを選択
	SelectObject( hdc, hFont );

	// 現在選択されているフォントの取得
	GetTextMetrics( hdc, &m_tm );

	//最大バッファサイズ
	DWORD dwMaxBufferSize = 0;

	//テクスチャサイズ
	int width = 0;
	int height = m_tm.tmHeight;
	
	// 現在の文字へのポインタ
	const BYTE *ptr = ( const BYTE*)str;

	//テクスチャサイズ用ループ
	while(true)
	{
		// ストリング内の文字を取得
		UINT nChar = _mbsnextc( ptr );
		
		//文字列終了（NULL終端）
		if(nChar == '\0')
			break;

		// 文字描画用行列
		MAT2 mat2 = 
		{
			{ 0, 1, }, 
			{ 0, 0, },
			{ 0, 0, }, 
			{ 0, 1, }
		};

		// バッファサイズの取得
		DWORD dwBufferSize = GetGlyphOutline( hdc, nChar, GGO_GRAY8_BITMAP, &pTexture->m_gm, 0, NULL, &mat2 );
		
		//バッファサイズを更新
		if(dwBufferSize > dwMaxBufferSize)
			dwMaxBufferSize = dwBufferSize;

		//幅を更新
		width += pTexture->m_gm.gmCellIncX;

		// 次の文字へ移動
		ptr = _mbsinc( ptr );
	}

	// バッファの作成
	LPBYTE hBuffer = (LPBYTE)GlobalAlloc(GPTR, dwMaxBufferSize);

	if(hBuffer == NULL)
		return E_FAIL;

	width = ceil2( width );
	height = ceil2( height );
	int size = ceil2(max(width,height));
	//正方形オプションで場合わけ
	if( !Sun3D.m_square )
		pTexture->m_tex.Create( width, height, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);
	else
		pTexture->m_tex.Create( size, size, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);

	//テクスチャをロック
	D3DLOCKED_RECT d3dRect;
	hr = pTexture->m_tex.m_pd3dtex->LockRect(0,&d3dRect,NULL,0);

	if(FAILED(hr))
		return hr;

	//ロックされた領域へのポインタ
	BYTE* pBuffer = (BYTE*)d3dRect.pBits;
	DWORD* pdwBuffer = (DWORD*)d3dRect.pBits;

	//文字描画位置
	int mojiX = 0;
	int mojiY = 0;

	// 現在の文字へのポインタ
	ptr = ( const BYTE*)str;

	//文字ごとにループ
	while(true)
	{
		// ストリング内の文字を取得
		UINT nChar = _mbsnextc( ptr );
		
		//文字列終了（NULL終端）
		if(nChar == '\0')
			break;

		// 文字描画用行列
		MAT2 mat2 = 
		{
			{ 0, 1, }, 
			{ 0, 0, },
			{ 0, 0, }, 
			{ 0, 1, }
		};

		// バッファサイズの取得
		DWORD dwBufferSize = GetGlyphOutline( hdc, nChar, GGO_GRAY8_BITMAP, &pTexture->m_gm, 0, NULL, &mat2 );

		//空白文字でない場合
		if(iswspace(nChar) == 0)
		{
			if(dwBufferSize == 0)
				return E_FAIL;

			// 実際にバッファに取得
			// アウトラインをグレースケールで作成
			DWORD dwResult = GetGlyphOutline( hdc, nChar, GGO_GRAY8_BITMAP, &pTexture->m_gm, dwBufferSize, ( LPVOID)hBuffer, &mat2 );

			if(dwResult == GDI_ERROR)
				return E_FAIL;

			// 文字の高さと幅の取得
			int dw = pTexture->m_gm.gmBlackBoxX;
			int dh = pTexture->m_gm.gmBlackBoxY;

			// ピッチをもとめます（３２ビット区切り）
			int pitch = ( pTexture->m_gm.gmBlackBoxX + 3 ) & 0xfffc;

			// 左上隅からの開始位置の取得
			int offsetX = mojiX + pTexture->m_gm.gmptGlyphOrigin.x;
			int offsetY = mojiY + m_tm.tmAscent - pTexture->m_gm.gmptGlyphOrigin.y;

			// 文字の幅と高さの数だけループ
			for( int i = 0; i < dh; i++ )
			{
				for( int ii = 0; ii < dw; ii++ )
				{
					BYTE alpha, *p_alpha;		// アルファ値

					// 色の取得
					p_alpha = ( (( LPBYTE)hBuffer + ( i * pitch ) + ii ));
					alpha = *( p_alpha );

					// 得られたビットマップは（なぜか）0-64の６５段階なので0-255に変換
					if(alpha != 0)
					{
						alpha = (alpha-1) * 4;
					}

					//色の代入
					BYTE a = color>>24 & 0xff;
					BYTE r = color>>16 & 0xff;
					BYTE g = color>>8 & 0xff;
					BYTE b = color & 0xff;

					pdwBuffer[ (i+offsetY) * d3dRect.Pitch/4 + (ii+offsetX) ] = D3DCOLOR_ARGB(alpha * a / 255, r, g, b);
				}
			}
		}

		// 次の文字の描画位置へ移動
		mojiX += pTexture->m_gm.gmCellIncX;
		mojiY += pTexture->m_gm.gmCellIncY;

		// 次の文字へ移動
		ptr = _mbsinc( ptr );
	}
	// １文字分のバッファの解放
	GlobalFree( hBuffer );

	// フォントの解放
	DeleteObject( hFont );

	//テクスチャのアンロック
	pTexture->m_tex.m_pd3dtex->UnlockRect(0);

	//デバイスコンテキストの開放
	ReleaseDC( hDskWnd, hDskDC );
	DeleteDC( hdc );

	return S_OK;
}

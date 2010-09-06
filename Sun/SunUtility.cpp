/**
	@file SunUtility.cpp
	@brief ユーティリティ
*/
#include "stdafx.h"
#include "SunUtility.h"

/**
	@brief カレントディレクトリ

	カレントを取得します。
	実行ファイルの位置にカレントを設定し、設定したカレントを返します。
*/
string GetCurrent()
{
	char buffer[MAX_PATH];
	::GetModuleFileName( NULL, buffer, MAX_PATH );
	char* p = strrchr(buffer,'\\');
	if(p != NULL)
		p[0] = '\0';
	::SetCurrentDirectory(buffer);
	return (string)buffer;
}

/**
	@brief カレントディレクトリ

	カレントを実行イメージの位置に設定します。
*/
void ResetCurrent()
{
	char buffer[MAX_PATH];
	::GetModuleFileName( NULL, buffer, MAX_PATH );
	char* p = strrchr(buffer,'\\');
	if(p != NULL)
		p[0] = '\0';
	::SetCurrentDirectory(buffer);
}
/*
	補助関数
*/
DWORD   GetMouseParam(int nXY,bool bX)
{
	int		nSize;

	if(bX == true)
		nSize = ::GetSystemMetrics(SM_CXSCREEN);
	else
		nSize = ::GetSystemMetrics(SM_CYSCREEN);

	return  nXY * (65535 / nSize);
}

/*
	マウスカーソルの位置を設定します。
*/
void	MoveMouse(int nX,int nY)
{
	nX = GetMouseParam(nX,true);
	nY = GetMouseParam(nY,false);

	::mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,nX,nY,NULL,NULL);
}

//矩形同士の当たり判定
int GetHantei(RECT *r1,RECT *r2)
{
	if( r1->left > r2->right ||
		r1->right < r2->left ||
		r1->top > r2->bottom ||
		r1->bottom < r2->top ) {
		return 0;
	}
	return 1;
}
//フォルダ作成（スナップショット・リプレイ用）
int CreateDirectory( char* name )
{
	int result = CreateDirectory(name,NULL);
	SetFileAttributes( name, FILE_ATTRIBUTE_NORMAL );
	return result;
}
int pown( int x, int n ){
	int result = 1;
	for( int i = 0; i < n; ++ i ) {
		result *= x;
	}
	return result;
}

// SunTask.cpp: SunTask クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunTask.h"

/**
	@brief タスク
*/
SunTask::SunTask()
{
}
SunTask::~SunTask()
{
}
/**
	@brief タスクリスト
*/
SunTaskList::SunTaskList()
{
}
SunTaskList::~SunTaskList()
{

}

int SunTaskList::operator ()()
{
	SunTaskListItr i;
	for(i=m_lstTask.begin();i!=m_lstTask.end();++i)
	{
		//ＮＵＬＬチェック
		if(*i != NULL)
		{
			**i;
			if((**i)() == 0)	//タスク実行
			{
				delete(*i);
				return 0;
			}
		}
	}
	return 1;
}
int SunTaskList::Clear()
{
	this->m_lstTask.clear();
	return 1;
}
//全削除
int SunTaskList::Delete()
{
	SunTaskListItr i;
	for(i=m_lstTask.begin();i!=m_lstTask.end();++i)
	{
		//ＮＵＬＬチェック
		if(*i != NULL)
		{
			delete(*i);
		}
	}
	this->Clear();
	return 1;
}
//タスク追加
int SunTaskList::operator+=(SunTask* pTask)
{
	this->m_lstTask.push_back(pTask);
	return 1;
}
/**
	@brief タスク配列
*/
SunTaskArray::SunTaskArray()
{

}

SunTaskArray::~SunTaskArray()
{

}
int SunTaskArray::operator ()()
{
	SunTaskArrayItr i;
	for(i=m_vecTask.begin();i!=m_vecTask.end();++i)
	{
		//ＮＵＬＬチェック
		if(*i != NULL)
		{
			if((**i)() == 0)	//タスク実行
			{
				delete(*i);
				return 0;
			}
		}
	}
	return 1;
}
//ランダムアクセス
SunTask*& SunTaskArray::operator[](int i)
{
	return this->m_vecTask[i];
}
//タスク追加
int SunTaskArray::operator+=(SunTask* pTask)
{
	this->m_vecTask.push_back(pTask);
	return 1;
}
/**
	@brief レイヤータスク
*/
SunLayerTask::SunLayerTask()
{

}

SunLayerTask::~SunLayerTask()
{

}
int SunLayerTask::operator ()()
{
	for(int i=0; i<this->m_vecTask.size(); ++i)
	{
		//ポインタではないのでＮＵＬＬチェック不要
		{
			if(m_vecTask[i]() == 0)	//タスクリスト実行
				return 0;
		}
	}
	return 1;
}

//ランダムアクセス
SunTaskList& SunLayerTask::operator[](int i)
{
	if(i<0 || i>= m_vecTask.size())
	{
		::MessageBox(NULL,"エラー","配列のアクセス範囲違反",MB_OK|MB_ICONERROR);
		throw;
	}
	return this->m_vecTask[i];
}

//サイズ取得
int SunLayerTask::Size()
{
	return this->m_vecTask.size();
}
//サイズ設定
int SunLayerTask::Resize(int size)
{
	this->m_vecTask.resize(size);
	return 1;
}
//クリア
int SunLayerTask::Clear()
{
	this->m_vecTask.clear();
	return 1;
}
//全削除
int SunLayerTask::Delete()
{
	for(int i=0; i<m_vecTask.size(); ++i)
	{
		//ポインタではないのでＮＵＬＬチェック不要
		{
			m_vecTask[i].Delete();
		}
	}
	return 1;
}

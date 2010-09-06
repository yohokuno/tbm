// SunTask.h: SunTask クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SunUtility.h"

class SunLayerTask;
/**
	@brief タスク

	タスクのインターフェースクラスです。
	関数オブジェクトとして機能します。
	というか、汎用のfanctorといっていいです。
*/
class SunTask  
{
public:
	SunTask();
	virtual ~SunTask();
	virtual int operator()() = 0;		///<関数オブジェクトとして機能
};
/**
	@brief タスクリスト

	タスクのリストです。
	これ自体もタスクです。
	実際に使うときは、シーンクラスのメンバとして定義すれば良いでしょう。
	ただし、描画タスクだけは全体で１つです。
*/
class SunTaskList : public SunTask
{
private:
	list<SunTask*> m_lstTask;	///<タスクリスト
public:
	SunTaskList();
	virtual ~SunTaskList();
	virtual int operator()();
	int operator+=(SunTask* pTask);	///<タスク追加
	int Clear();
	int Delete();
};
typedef list<SunTask*>::iterator SunTaskListItr;
/**
	@brief タスク配列

	タスクのＶｅｃｔｏｒ配列です。
	リストと違うのは、「ランダムアクセスできること」と「基本的にサイズが決まっていること」
	使い道ないかも。。。
*/
class SunTaskArray : public SunTask
{
private:
	vector<SunTask*> m_vecTask;	///<タスク配列
public:
	SunTaskArray();
	virtual  ~SunTaskArray();
	virtual int operator()();
	SunTask*& operator[](int i);
	int operator+=(SunTask* pTask);	///<タスク追加
};
typedef vector<SunTask*>::iterator SunTaskArrayItr;
/**
	@brief レイヤータスク

	レイヤーを実現する、「タスクリストの配列」です。
	描画の順序を入れ替えれますが、描画に限らず順序を決める必要のあるタスク集合
	に対して使うことができると思います。
*/
class SunLayerTask : public SunTask
{
private:
	vector<SunTaskList> m_vecTask;	///<タスク配列
public:
	SunLayerTask();
	virtual ~SunLayerTask();
	virtual int operator()();
	SunTaskList& operator[](int i);
	int Size();
	int Resize(int size);
	int Clear();
	int Delete();
};
typedef vector<SunTaskList>::iterator SunLayerTaskItr;


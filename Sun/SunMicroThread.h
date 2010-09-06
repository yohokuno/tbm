#pragma once

#include "SunFunctionCallback.h"

class SunMicroThread {
public:

	void	Start(SunFunctionCallback *fn,int nStackSize=0x8000);
	void	Suspend();		///	実行中の処理を中断する
	void	Suspend(int n);	/// 実行中の処理をn回中断する。Suspendをn回呼び出すと考えれば良い
	void	Resume();		///	中断した処理を再開する
	void	Resume(int n);	///	中断した処理を再開する

	///	サスペンド中か？
	bool	IsSuspended() {
		return m_bSuspended;
	}

	///	マイクロスレッドの実行は終了したか？
	bool	IsEnd() {
		return m_bEnd;
	}

	SunMicroThread(){
		m_bSuspended = false;
		m_bEnd = false;
	}

protected:
	vector<BYTE>	m_aStack;	//	スタック
	SunFunctionCallback* m_fn; // コールバック先のfunction

	bool			m_bSuspended;	//	マイクロスレッドのサスペンド
	bool			m_bEnd;			//	マイクロスレッドの終了フラグ
	BYTE*			m_register_esp;			//	記録しているesp

	void	SwitchThread();
	static SunMicroThread*	InnerCallBack(SunMicroThread*p);
};

class SunMicroThreadEx : public SunMicroThread{
public:
	double m_t1;
	double m_t2;
	int m_stop;
	SunMicroThreadEx() {
		m_t1 = 0;
		m_t2 = 0;
		m_stop = 0;
	}
	void	Start(SunFunctionCallback *fn,int nStackSize=0x8000) {
		m_t1 = 0;
		m_t2 = 0;
		m_stop = 0;
		SunMicroThread::Start(fn,nStackSize);
	}
	void Suspend( double t ) {
		double sum = m_t1 + t;
		int	num = (int)sum;	//小数点以下切り捨て
		m_t1 = sum - num;	//切り捨て部分
		SunMicroThread::Suspend( num );
	}
	void Resume( double t ) {
		if( m_stop ) return;
		double sum = m_t2 + t;
		int	num = (int)sum;	//小数点以下切り捨て
		m_t2 = sum - num;	//切り捨て部分
		SunMicroThread::Resume( num );
	}
	void Suspend() {
		m_t1 = 0;
		SunMicroThread::Suspend();
	}
	void Resume() {
		if( m_stop ) return;
		m_t2 = 0;
		SunMicroThread::Resume();
	}
	void Stop() { m_stop = 1; }
	int IsStop() { return m_stop; }
};
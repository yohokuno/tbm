#include "stdafx.h"
#include "SunMicroThread.h"

void	SunMicroThread::Start(SunFunctionCallback *fn,int nStackSize)
{
	m_bEnd = false;
	m_aStack.resize(nStackSize);
	m_fn = fn;
	m_register_esp = &m_aStack[0]+nStackSize;

	void *fInnerCallBack = (void*)&SunMicroThread::InnerCallBack;
	//	VC++6のインラインアセンブラがstaticなメンバ関数のcallを
	//	許可していないため(VC++7では可能)

	_asm {
		mov		eax,[this]
		mov		ecx,fInnerCallBack

		push	offset Next	//	戻り値のpush

		push	ebx
		push	ebp
		push	esi
		push	edi

		xchg	[eax].m_register_esp , esp

		//	bpを切り替えているので、this自体が不正になるので
		//	ここ以降、メンバにアクセスすら出来ない
		push	eax	//	InnerCallBackの第１パラメータとして渡す
//			call	InnerCallBack
//	VC++6ではオーバーロードされていないグローバル関数しかインラインアセンブラから呼び出せない(;´Д`)
		call	ecx

	//	ここに到達したということは、その関数の実行が終了したということを意味するので
	//	スタックを元の状態に戻してやる必要がある

		//	thisをどうやって獲得するのか..が問題だが、InnerCallBackが戻り値としてthisを
		//	返してあれば良い
		xchg	[eax].m_register_esp , esp
		mov		[eax].m_bEnd,1	//	bEnd_ = true;

		pop		edi
		pop		esi
		pop		ebp
		pop		ebx

//			pop		eax	//	最初に積んだラベルも
//			jmp		eax
		ret
Next:
	}
}

void	SunMicroThread::Suspend()	///	実行中の処理を中断する
{
	m_bSuspended = true;

	SwitchThread();
}

#pragma warning(disable:4731)	//	ebp変更したぐらいでｶﾞﾀｶﾞﾀ言うなや(ﾟдﾟ)ｺﾞﾙｧ
void	SunMicroThread::SwitchThread(){
	_asm {
		//	現在の仮想スタックに積む
		push	offset Next
		//	resumeのあとここに戻ってくる必要があるので

		push	ebx
		push	ebp
		push	esi
		push	edi

		mov		eax,[this]
		xchg	esp,[eax].m_register_esp

		pop		edi
		pop		esi
		pop		ebp
		pop		ebx

//			pop		eax
//			jmp		eax
		ret
Next:;
	}
}

SunMicroThread* SunMicroThread::InnerCallBack(SunMicroThread*p){
	//	fn_にコールバックする
	p->m_fn->Call();	//	fn_にコールバックする
	return p;
}	

void	SunMicroThread::Resume()		///	中断した処理を再開する
{
	if (!IsSuspended()) return ;	//	中断（･Ａ･）ｼﾃﾅｲ!!

	m_bSuspended = false;

	SwitchThread();
}

void	SunMicroThread::Suspend(int n){
	while (n-->0){
		Suspend();
	}
}

void	SunMicroThread::Resume(int n){
	while (n-->0){
		Resume();
	}
}

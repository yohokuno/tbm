/**
	@file SunRand.h
	@brief 擬似乱数生成クラス

    Mersenne Twister法による擬似乱数。
	クラスにした事で、複数の乱数系列を扱えます。
	ほぼYandSDK3のCRandです。
*/

#pragma once

#define MAX_RAND (0xffffffff)

class CSunRand {
public:
	DWORD	Get();			//	乱数の取得
	DWORD	Get(DWORD n) {	//	0～n-1の乱数の取得
		if (n==0) { 
			return 0;
		}
		return Get() % n;
	}

	void	SetSeed(DWORD dwSeed);
	void	Randomize()	{SetSeed(::GetTickCount());}

	CSunRand() { m_nMti = 624+1; }
	CSunRand(DWORD dwSeed) { SetSeed(dwSeed); }
	virtual ~CSunRand() {}
protected:
	DWORD	m_dwMt[624];	// the array for the state vector
	int		m_nMti;			// initialization counter
};

extern CSunRand SunRand;	// リプレイに影響する目的
extern CSunRand SunRand2;	// リプレイ無関係（エフェクト用）

// 補助関数
inline double rand(double min,double max){return min + (max-min) * SunRand.Get() / MAX_RAND;}
inline int irand(int min,int max){return min + SunRand.Get(max-min+1);}
inline double rand2(double min,double max){return min + (max-min) * SunRand2.Get() / MAX_RAND;}
inline int irand2(int min,int max){return min + SunRand2.Get(max-min+1);}

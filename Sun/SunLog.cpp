// SunLog.cpp: CSunLog クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunLog.h"

CSunLog		SunLog;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CSunLog::CSunLog()
{
	//ログ関係
	this->m_logfilename	= "log.txt";
}

CSunLog::CSunLog(const char* file)
{
	this->m_logfilename = file;
	this->LogStart();
}
CSunLog::~CSunLog()
{

}

/**
	初期化（stderrのリダイレクト）
*/
int CSunLog::LogStart()
{
	//ログファイルへstderrをリダイレクト
	freopen(this->m_logfilename.data(),"w",stderr);
	return 1;
}
/*
	@brief ログ出力
	printfの書式でthis->m_logfilenameへ出力
*/
int CSunLog::LogOut(const char *fmt,...)
{
	if(fmt == NULL)
		return 0;
	va_list arg;
	va_start(arg,fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);

	//直ちに書き込み（フラッシュ）
	fflush(stderr);
	return 1;
}
/**
	@brief ログ出力
	ある値が１か０かで出力
*/
int CSunLog::LogCaps(int i,char *text)
{
	this->LogOut("%s [ %s ]\n",text,i ? "○" : "×");
	return 1;
}
/**
	@brief （）演算子
*/
int CSunLog::operator()(const char *fmt,...)
{
	if(fmt == NULL)
		return 0;
	va_list arg;
	va_start(arg,fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);
	return 1;
}
/*-----------------------------------------------------------------------------
	ログ出力初期化とシステム情報出力
-----------------------------------------------------------------------------*/
int CSunLog::GetAllInformation()
{
	// ＣＰＵ情報の取得
	this->GetCPUInformation();

	// ＯＳ情報の取得-
	this->GetOSInformation();

	// メモリ情報の取得
	this->GetMemoryInformation();

	// システム情報の取得
	this->GetSystemInformation();
	return 1;
}
/******************************************************************************

  *******     *       **     **      **
        *     ****** *  *    * **   *  *
		*     *       **    *    **  **
        *     *            *      **
  *******      *****      *         **

******************************************************************************/
//=============================================================================
/**	ＣＰＵ情報の取得
	ＣＰＵの能力を取得します
	Ｌｕｎａのコピペｗ
*/
//=============================================================================
int CSunLog::GetCPUInformation()
{

	char CPUName[256]	= "";
	char CPUType[128]	= "";
	long bFPU			= FALSE;
	long bTSC			= FALSE;
	long bCMOV			= FALSE;
	long bFCMOV			= FALSE;
	long bCPUID			= FALSE;
	long bMMX			= FALSE;
	long bMMX2			= FALSE;
	long bSSE			= FALSE;
	long bSSE2			= FALSE;
	long b3DNOW			= FALSE;
	long bE3DNOW		= FALSE;
	long TypeID			= 0;
	long FamilyID		= 0;
	long ModelID		= 0;
	long SteppingID		= 0;
	long CPUClock		= 0;

	__asm
	{
		//-----------------------------------------------------------
		// CPUID命令の存在チェック
		//-----------------------------------------------------------
		PUSHFD
		POP		EAX
		MOV		EBX,		EAX
		XOR		EAX,		1<<21
		PUSH	EAX
		POPFD
		PUSHFD
		POP		EAX
		CMP		EAX,		EBX
		JE		EXIT				// ない
		MOV		bCPUID,		1

		//-----------------------------------------------------------
		// CPUID 0
		//-----------------------------------------------------------
		MOV		EAX,		0
		CPUID

		CMP		EAX,		0
		JE		EXIT				// 0では話にならん

		MOV DWORD PTR [CPUType+0],	EBX
		MOV DWORD PTR [CPUType+8],	ECX
		MOV DWORD PTR [CPUType+4],	EDX

		//-----------------------------------------------------------
		// CPUID 1
		//-----------------------------------------------------------
		MOV		EAX,		1
		CPUID

		//----------------------------------------------
		// EAXレジスタの中身検証
		//----------------------------------------------
		// ステッピングID
		MOV		ESI,		EAX
		AND		ESI,		0x0F;		// 下位4バイトにステッピングID
		MOV		[SteppingID],ESI

		// モデルID
		SHR		EAX,		4			// 右ステッピングID分に詰めて･･･
		MOV		ESI,		EAX
		AND		ESI,		0x0F		// さらに4バイトに
		MOV		[ModelID],	ESI

		// ファミリーID
		SHR		EAX,		4			// また詰めて･･･
		MOV		ESI,		EAX
		AND		ESI,		0x0F		// おなじみ4バイト
		MOV		[FamilyID],	ESI

		// タイプID
		SHR		EAX,		4			// もうええがな
		MOV		ESI,		EAX
		AND		ESI,		0x03		// 今度は2バイト
		MOV		[TypeID],	ESI

		//----------------------------------------------
		// EDXレジスタの中身検証
		//----------------------------------------------
		// FPU（なかったらヤバイと思うけどね＾＾；
		XOR		EAX,		EAX			// ゼロクリア
		TEST	EDX,		1<<0		// 1バイト目チェック
		SETNZ	AL						// 0でないならALレジスタへ転送
		MOV		[bFPU],		EAX			// 結果やいかに

		// TSC
		TEST	EDX,		1<<4
		SETNZ	AL
		MOV		[bTSC],		EAX

		// 条件付転送命令および比較
		XOR		EAX,		EAX
		TEST	EDX,		1<<15
		SETNZ	AL
		MOV		[bCMOV],	EAX

		// MMX命令
		XOR		EAX,		EAX
		TEST	EDX,		1<<23
		SETNZ	AL
		MOV		[bMMX],		EAX

		// MMX2 & SSE 命令
		XOR		EAX,		EAX
		TEST	EDX,		1<<25
		SETNZ	AL
		MOV		[bMMX2],	EAX
		MOV		[bSSE],		EAX

//////////////////////////////////
//		以下はAMDのCPU情報		//
//////////////////////////////////

		//-----------------------------------------------------------
		// CPUID 0x80000000
		//-----------------------------------------------------------
		MOV		EAX,		0x80000000
		CPUID

		CMP		EAX,		0x80000001	// 次のCPUID命令のサポートチェック
		JB		EXIT

		//-----------------------------------------------------------
		// CPUID 0x80000001
		//-----------------------------------------------------------
		MOV		EAX,		0x80000001
		CPUID

		// MMX2
		XOR		EAX,		EAX
		TEST	EDX,		1<<22
		SETNZ	AL
		MOV		[bMMX2],	EAX

		// Enhansed 3DNow!
		XOR		EAX,		EAX
		TEST	EDX,		1<<30
		SETNZ	AL
		MOV		[bE3DNOW],	EAX

		// Enhansed 3DNow!
		XOR		EAX,		EAX
		TEST	EDX,		1<<31
		SETNZ	AL
		MOV		[b3DNOW],	EAX

		//-----------------------------------------------------------
		// CPUID 0x80000002 - 0x80000004
		//-----------------------------------------------------------
		// サポートしてるか？
		MOV		EAX,		0x80000000
		CPUID
		CMP		EAX,		0x80000004
		JB		EXIT

		// 0x80000002
		MOV EAX, 0x80000002
		CPUID

		MOV DWORD PTR [CPUName+ 0],		EAX
		MOV DWORD PTR [CPUName+ 4],		EBX
		MOV DWORD PTR [CPUName+ 8],		ECX
		MOV DWORD PTR [CPUName+12],		EDX

		// 0x80000003
		MOV EAX, 0x80000003
		CPUID

		MOV DWORD PTR [CPUName+16],		EAX
		MOV DWORD PTR [CPUName+20],		EBX
		MOV DWORD PTR [CPUName+24],		ECX
		MOV DWORD PTR [CPUName+28],		EDX

		// 0x80000004
		MOV EAX, 0x80000004
		CPUID

		MOV DWORD PTR [CPUName+32],		EAX
		MOV DWORD PTR [CPUName+36],		EBX
		MOV DWORD PTR [CPUName+40],		ECX
		MOV DWORD PTR [CPUName+44],		EDX
		
	// 終了
	EXIT:
	}


	//------------------------------------------------------------------------
	// CPUクロック取得
	//------------------------------------------------------------------------
	if ( bTSC )
	{
		__asm
		{
			RDTSC
			MOV		[CPUClock],		EAX
		}

		Sleep( 1000 );

		__asm
		{
			RDTSC
			SUB		EAX,			[CPUClock]
			MOV		[CPUClock],		EAX
		}

		CPUClock /= 1000000;
	}

	//--------------------------------------------------
	// チェック完了～
	//--------------------------------------------------

	if ( (strlen(CPUType)>0) && (strlen(CPUName)>0) )
	{
		strcat( CPUName, " - " );
		strcat( CPUName, CPUType );
	}
	else
	{
		strcpy( CPUName, "UnKnown" );
	}

	// ログ
	this->LogOut( "ＣＰＵ情報\n" );
	this->LogOut( "　　　Name [ %s ]\n", CPUName );
	this->LogOut( "　　　Clock [ %uMB ]\n", CPUClock );
	this->LogOut( "　　　Type [ %u ] Family [ %u ] Model [ %u ] Stepping [ %u ]\n", TypeID, FamilyID, ModelID, SteppingID );
	this->LogOut( "　　　FPU %s : CPUID %s : MMX %s\n", (bFPU  )?("○"):("×"), (bCPUID )?("○"):("×"), (bMMX )?("○"):("×") );
	this->LogOut( "　　　MMX2 %s : SSE %s : SSE2 %s\n", (bMMX2 )?("○"):("×"), (bSSE   )?("○"):("×"), (bSSE2)?("○"):("×") );
	this->LogOut( "　　　3DNow %s : E3DNow %s : CMOV %s\n", (b3DNOW)?("○"):("×"), (bE3DNOW)?("○"):("×"), (bCMOV)?("○"):("×") );
	this->LogOut( "　　　FCMOV %s : TSC %s\n", (bFCMOV)?("○"):("×"), (bTSC   )?("○"):("×") );

	return 1;
}
//=============================================================================
/**	ＯＳの取得
	ＯＳの種類とバージョンを取得します。
	これもＬｕｎａのコピペｗ
*/
//=============================================================================
int CSunLog::GetOSInformation()
{
	OSVERSIONINFOEX OSVer;
	BOOL IsVersionEx;

	char StrType[256]		= "";
	char StrVersion[256]	= "";
	char StrBuild[256]		= "";

	//---------------------------------------------------------------------
	// OSのバージョン取得
	//---------------------------------------------------------------------
	memset( &OSVer, 0x00, sizeof(OSVERSIONINFOEX) );
	OSVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	IsVersionEx = GetVersionEx( (OSVERSIONINFO*)&OSVer );

	// ＥＸ系の関数はだめなようで・・・
	if ( !IsVersionEx )
	{
		OSVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if ( !GetVersionEx( (OSVERSIONINFO*)&OSVer ) )
		{
			this->LogOut( "Operation System [ Unknown ]" );
			return 1;
		}
	}

	//---------------------------------------------------------------------
	// バージョン
	//---------------------------------------------------------------------
	sprintf( StrVersion, "Version %d.%d", OSVer.dwMajorVersion, OSVer.dwMinorVersion );

	//---------------------------------------------------------------------
	// こんなＯＳありえるのだろうか・・・
	//---------------------------------------------------------------------
	if ( OSVer.dwPlatformId == VER_PLATFORM_WIN32s )
	{
		sprintf( StrVersion, "Windows 3.1 ( Build %d %s )", OSVer.dwBuildNumber, OSVer.szCSDVersion );
	}
	//---------------------------------------------------------------------
	// ＷｉｎｄｏｗＮＴ系
	//---------------------------------------------------------------------
	else if ( OSVer.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		// ＯＳ名
		switch ( OSVer.dwMajorVersion )
		{
		case 3:
			strcpy( StrType, "WindowsNT3.5" );
			break;
		case 4:
			strcpy( StrType, "WindowsNT4" );
			break;
		case 5:
			switch ( OSVer.dwMinorVersion )
			{
			case 0:
				strcpy( StrType, "Windows2000" );
				break;
			case 1:
				strcpy( StrType, "WindowsXP" );
				break;
			}
			break;
		}
#ifdef __VISUAL_STUDIO_NET__
		// ＯＳ拡張情報
		if ( IsVersionEx )
		{
			if( OSVer.wProductType == VER_NT_WORKSTATION )
			{
				if ( OSVer.wSuiteMask & VER_SUITE_PERSONAL )
				{
					strcat( StrType, " - Home Edition" );
				}
				else
				{
					strcat( StrType, " - Professional" );
				}
			}
			else
			{
				if ( OSVer.wProductType == VER_NT_SERVER )
				{
					if ( OSVer.wSuiteMask & VER_SUITE_DATACENTER )
					{
						strcat( StrType, " - DataCenter Server" );
					}
					else
					{
						if( OSVer.wSuiteMask & VER_SUITE_ENTERPRISE )
						{
							if ( OSVer.dwMajorVersion == 4 )
							{
								strcat( StrType, " - Advanced Server" );
							}
							else
							{
								strcat( StrType, " - Enterprise Server" );
							}
						}
						else
						{
							if ( OSVer.wSuiteMask == VER_SUITE_BLADE )
							{
								strcat( StrType, " - Web Server" );
							}
							else
							{
								strcat( StrType, " - Server" );
							}
						}
					}
				}
			}
		}
#endif // __VISUAL_STUDIO_NET__
		// バージョン
		sprintf( StrBuild, "Build %d / %s", OSVer.dwBuildNumber, OSVer.szCSDVersion );	
	}
	//---------------------------------------------------------------------
	// Ｗｉｎｄｏｗ９ｘ系
	//---------------------------------------------------------------------
	else if ( OSVer.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		if ( OSVer.dwMinorVersion == 0 )
		{
			strcpy( StrType, "Windows95" );
			if( (OSVer.szCSDVersion[1] == 'C') || (OSVer.szCSDVersion[1] == 'B') )
			{
				strcat( StrType, " - OSR2 " );
			}
		}
		else if ( OSVer.dwMinorVersion < 90 )
		{
			strcpy( StrType, "Windows98" );
			if ( OSVer.szCSDVersion[1] == 'A' )
			{
				strcat( StrType, " - SE " );
			}
		}
		else
		{
			strcpy( StrType, "WindowsMe" );
		}

		// バージョン
		sprintf( StrBuild, "( Build %d / %d )", HIWORD( OSVer.dwBuildNumber ), LOWORD( OSVer.dwBuildNumber ) );
	}
	//---------------------------------------------------------------------
	// わけわからんち
	//---------------------------------------------------------------------
	else
	{
		sprintf( StrType, "Windows ( %d )", OSVer.dwPlatformId );
	}

	// ログ
	this->LogOut( "ＯＳ情報\n" );
	this->LogOut( "　　　Type [ %s ]\n", StrType );
	this->LogOut( "　　　Version [ %s ]\n", StrVersion );
	this->LogOut( "　　　Build [ %s ]\n", StrBuild );
	return 1;
}
//=============================================================================
/**	メモリ情報の設定
	メモリ情報の取得をします。
*/
//=============================================================================
int CSunLog::GetMemoryInformation()
{
	this->LogOut( "メモリ情報\n" );

	//----------------------------------------------------------
	// メモリ容量取得
	//----------------------------------------------------------
	MEMORYSTATUS MemState = { sizeof(MEMORYSTATUS) };
	::GlobalMemoryStatus( &MemState );

	this->LogOut( "　　　Total [ %.3lfMB ]", MemState.dwTotalPhys / 1024.0 / 1024.0 );
	this->LogOut( "　　　Free [ %.3lfMB ]", MemState.dwAvailPhys / 1024.0 / 1024.0 );
	this->LogOut( "　　　Used [ %u%% ]\n", MemState.dwMemoryLoad );

	this->LogOut( "　　　Total PageFile [ %.3lfMB ]", MemState.dwTotalPageFile / 1024.0 / 1024.0 );
	this->LogOut( "　　　Free PageFile [ %.3lfMB ]\n", MemState.dwAvailPageFile / 1024.0 / 1024.0 );

	this->LogOut( "　　　Total Virtual [ %.3lfMB ]", MemState.dwTotalVirtual / 1024.0 / 1024.0 );
	this->LogOut( "　　　Free Virtual [ %.3lfMB ]\n", MemState.dwAvailVirtual / 1024.0 / 1024.0 );

	return 1;
}
//=============================================================================
/**	システム情報
	システムの情報を取得します。
*/
//=============================================================================
int CSunLog::GetSystemInformation()
{
	this->LogOut( "システム情報\n" );

	//---------------------------------------------------------------------
	// 画面情報
	//---------------------------------------------------------------------
	this->LogOut( "　	Screen Mode" );
	this->LogOut( "　	[ %u x %u ]\n",
		::GetSystemMetrics( SM_CXSCREEN ),
		::GetSystemMetrics( SM_CYSCREEN ) );

	//---------------------------------------------------------------------
	// ブートモードの取得
	//---------------------------------------------------------------------
	switch( GetSystemMetrics( SM_CLEANBOOT ) )
	{
	case 0:
		this->LogOut( "	OS Boot Mode" );
		this->LogOut( "	[ Normal Boot ]\n" );
		break;
	case 1:
		this->LogOut( "	OS Boot Mode" );
		this->LogOut( "	[ Safe Mode ]\n" );
		break;
	case 2:
		this->LogOut( "	OS Boot Mode" );
		this->LogOut( "	[ Safe Mode with Network ]\n" );
		break;
	}
	return 1;
}


#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

CTitle *g_pTitle = NULL;
CTitleMaterial *g_pTitleMaterial = NULL;

CTitle::CTitle():
		sndDecide("snd/decide.wav"),
		sndCancel("snd/cancel.wav"),
		sndSelect("snd/select.wav")
{
	mode = 0;
	title_select = 0;
	difficult_select = DIFF_NORMAL;
	stage_select = 1;
	option_select = 0;
	config_select = 0;
	result_select = 0;
	replay_select = 0;
	spell_select	= 0;
	spell_difficult	= 1;
	inp = -1;
	inp_old = -1;
	otaku = SunConfig.GetInt2("system","otaku",1);
	replay_config = SunConfig.GetInt2("system","replay",1);
	talk = SunConfig.GetInt2("system","talk",1);

	sndDecide.SetVolume( 0 );
	sndCancel.SetVolume( 0 );
	sndSelect.SetVolume( -1000 );

	g_pTitleMaterial = new CTitleMaterial();
	g_pRecord = new CRecord;
}
CTitle::~CTitle()
{
	SAFE_DELETE( g_pRecord );
	SAFE_DELETE( g_pTitleMaterial );
}
int CTitle::Run()
{
	switch( mode ) {
	case 0: //タイトル
		{
			if( GetInputDecide() ) {
				switch(title_select){
				case 0: //Start
					mode = 1;
					sndDecide.Play(0);
					break;
				case 1: //Extra Start
					break;
				case 2: //Practice Start
					mode = 1;
					sndDecide.Play(0);
					break;
				case 3: //Replay
					{//ファイル数確認
						::ResetCurrent();
						::CreateDirectory("replay");
						WIN32_FIND_DATA FindFileData;
						HANDLE hFind;
						char filename[MAX_PATH];
						char kakutyosi[] = ".rpy";
						int i;
						replay_num = 0;
						for( i = 0; i < REPLAY_NUM; ++i ) {
							sprintf( filename, "replay/tbm%02d%s", i, kakutyosi );
							hFind = FindFirstFile( filename, &FindFileData );
							if( hFind == INVALID_HANDLE_VALUE ) {
								replay_exist[i] = 0;
							}
							else {
								replay_exist[i] = 1;
								replay_num ++;
							}
						}
						replay_select = 0;
					}
					mode = 3;
					sndDecide.Play(0);
					break;
				case 4: //Result
					mode = 4;
					sndDecide.Play(0);
					break;
				case 5: //Music Room
					break;
				case 6: //Option
					mode = 6;
					option_select = 0;
					optMode.Load("system","window",0);
					optGraphic.Load("direct3d","bitmode",16);
					optBGMVol.Load("sound","bgmvolume",BGMVOLUME);
					optSEVol.Load("sound","sevolume",SEVOLUME);
					optInterval.Load("direct3d","interval",1);
					sndDecide.Play(0);
					break;
				case 7: //Quit
					return 0;
				}
			}
			else if (GetInputCancel() ) {
				sndCancel.Play(0);
				title_select = 7;
			}
			int old = title_select;
			do{
				title_select += GetInputUD();
				title_select = mod( title_select, 8 );
			} while( title_select == 1 || title_select == 5 );
			if( title_select != old )
				sndSelect.Play(0);
		}
		break;
	case 1: // 難易度選択
		{
			if( GetInputDecide() ) {
				sndDecide.Play(0);
				switch( title_select ) {
				case 0://Start
					StartGame();
					return 1;
				case 2://Practice
					mode = 2;
					break;
				}
			}
			else if(GetInputCancel()) {
				sndCancel.Play(0);
				mode = 0;
			}
			int old = difficult_select;
			difficult_select += GetInputUD();
			difficult_select = mod( difficult_select, 4 );
			if( difficult_select != old )
				sndSelect.Play(0);
		}
		break;
	case 2: // ステージ選択
		{
			if( GetInputDecide() ) {
				if( stage_select >= 1 && stage_select <= STAGE_NUM ) {
					sndDecide.Play(0);
					StartGame();
					return 1;
				}
			}
			else if (GetInputCancel() ) {
				sndCancel.Play(0);
				mode = 1;
			}

			int old = stage_select;
			do{
				stage_select += GetInputUD();
				stage_select = mod( stage_select-1, STAGE_NUM+2 )+1;
			} while( stage_select > STAGE_NUM );
			if( stage_select != old )
				sndSelect.Play(0);
		}
		break;
	case 3: // リプレイ再生
		if( replay_num > 0 ) {
			if( GetInputDecide() ) {
				sndDecide.Play(0);
				StartGame();
				return 1;
			}
			else if (GetInputCancel() ) {
				sndCancel.Play(0);
				mode = 0;
			}
			int old = replay_select;
			replay_select += GetInputUD();
			replay_select += GetInputLR()*10;
			replay_select = mod( replay_select, REPLAY_NUM );
			if( replay_select != old ) {
				while( replay_exist[replay_select] == 0 ) {
					if( GetInputUD() + GetInputLR() > 0 )
						replay_select ++;
					if( GetInputUD() + GetInputLR() < 0 )
						replay_select --;
					replay_select = mod( replay_select, REPLAY_NUM );
				}
				sndSelect.Play(0);
			}
		}
		else if (GetInputCancel() ) {
			sndCancel.Play(0);
			mode = 0;
		}
		break;
	case 4: // Result
		{
			if( GetInputDecide() ) {
				sndDecide.Play(0);
				switch( result_select ) {
				case 0: // Score
					mode = 8;
					break;
				case 1: // Spell Card
					mode = 9;
					UpdateSpellFont();
					spell_sum = 0;
					for( int i = 0; i < SPELL_NUM; i++ ) {
						for( int j = 0; j < 4; j++ ) {
							int success = g_pRecord->data.spell[i][j].success;
							int challenge = g_pRecord->data.spell[i][j].challenge;
							if( success > 0 )
								spell_sum ++;
						}
					}
					break;
				}
			}
			else if (GetInputCancel() ) {
				sndCancel.Play(0);
				mode = 0;
			}
			int old = result_select;
			result_select += GetInputUD();
			result_select = mod( result_select, 2 );
			//result_select = mod( result_select, 1 );
			if( result_select != old )
				sndSelect.Play(0);
		}
		break;
	case 6: // オプション
		{
			if( GetInputDecide() || GetInputLR()) {
				switch( option_select ) {
				case 0: //Mode
					optMode.m_now = (optMode.m_now == 0) ? 1: 0;
					sndDecide.Play(0);
					break;
				case 1: //Graphic
					optGraphic.m_now = (optGraphic.m_now == 16) ? 32 : 16;
					sndDecide.Play(0);
					break;
				}
			}
			if( GetInputLRState() ) {
				switch( option_select ) {
				case 2: //BGMVol
					optBGMVol.m_now += 1*GetInputLRState();
					optBGMVol.m_now = max(0, min(100, optBGMVol.m_now));
					SunSound.m_bgmvolume = optBGMVol.m_now;
					SunAyame.m_pAyame->SetVolume( SunSound.m_bgmvolume );
					break;
				case 3: //SEVol
					optSEVol.m_now += 1*GetInputLRState();
					optSEVol.m_now = max(0, min(100, optSEVol.m_now));
					SunSound.m_sevolume = optSEVol.m_now;
					sndDecide.SetVolume( 0 );
					sndCancel.SetVolume( 0 );
					sndSelect.SetVolume( -1000 );
					break;
				}
				
			}
			//Interval
			if( GetInputLR() && option_select == 4 ) {
				optInterval.m_now += GetInputLR();
				optInterval.m_now = min(3,max(1,optInterval.m_now));
				sndDecide.Play(0);
			}
			if( GetInputDecide() ) {
				switch( option_select ) {
				case 5: //Reset
					optMode.m_now = 0;
					optGraphic.m_now = 16;
					optBGMVol.m_now = BGMVOLUME;
					optSEVol.m_now = SEVOLUME;
					sndDecide.Play(0);
					break;
				case 6: //KeyConfig
					mode = 7;
					config_select = 0;
					for( int i = 0; i < KEYCONFIG_NUM; ++i) {
						char buffer[32];
						sprintf( buffer, "Button%d", i);
						optKeyConfig[i].Load("Joystick0",buffer,i);
					}
					inp = SunInput.GetButtonData( );
					inp_old = inp;
					sndDecide.Play(0);
					break;
				case 7: //Quit
					{
						int restart = 0;
						mode = 0;
						restart |= optMode.Save();
						restart |= optGraphic.Save();
						optBGMVol.Save();
						optSEVol.Save();
						optInterval.Save();
						Sun3D.m_interval = optInterval.m_now;
						if( restart == 1 )
							SunApp.ReStart();
						else
							sndDecide.Play(0);
					}
					break;
				}
			}
			else if( GetInputCancel() ) {
				sndCancel.Play(0);
				option_select = 7;
			}
			int old = option_select;
			option_select += GetInputUD();
			option_select = mod( option_select, 8 );
			if( option_select != old )
				sndSelect.Play(0);
		}
		break;
	case 7: //キーコンフィグ
		{
			switch( config_select ) {
			case 8: //Quit
				if( GetInputDecide() ) {
					sndDecide.Play(0);
					mode = 6;
					int restart = 0;
					for( int i = 0; i < KEYCONFIG_NUM; ++i) {
						if( optKeyConfig[i].Save() )
							restart = 1;
					}
					if( restart ) {
						SunInput.ReInit();
					}
				}
				break;
			case 7: //Reset
				if( GetInputDecide()) {
					sndDecide.Play(0);
					for( int i = 0; i < KEYCONFIG_NUM; ++i) {
						optKeyConfig[i].m_now = i;
					}
				}
				break;
			default: //ButtonN
				inp_old = inp;
				inp = SunInput.GetButtonData( );
				if( inp != -1 && inp != inp_old && inp < INP_MAX) {
					int old = optKeyConfig[config_select].m_now;
					if( old != inp) {
						for( int i = 0; i < KEYCONFIG_NUM; ++i) {
							if( optKeyConfig[i].m_now == inp ) {
								optKeyConfig[i].m_now = old;
							}
						}
						sndDecide.Play(0);
						optKeyConfig[config_select].m_now = inp;
					}
				}
			}
			int old = config_select;
			config_select += GetInputUD();
			config_select = mod( config_select, 9 );
			if( config_select != old )
				sndSelect.Play(0);
		}
		break;
	case 8: // Score
		if( GetInputDecide() || GetInputCancel()) {
			sndCancel.Play(0);
			mode = 4;
		}
		break;
	case 9: // Spell Card
		{
			int old = spell_select;
			spell_select += GetInputUD();
			spell_select = mod( spell_select, SPELL_NUM / 10 + 1);
			if( spell_select != old ) {
				this->UpdateSpellFont();
				sndSelect.Play(0);
			}
			old = spell_difficult;
			spell_difficult += GetInputLR();
			spell_difficult = mod( spell_difficult, 4);
			if( spell_difficult != old ) {
				this->UpdateSpellFont();
				sndSelect.Play(0);
			}
			if( GetInputDecide() || GetInputCancel()) {
				sndCancel.Play(0);
				mode = 4;
			}
		}
		break;
	case 10: // Other Result
		if( GetInputDecide() || GetInputCancel()) {
			sndCancel.Play(0);
			mode = 4;
		}
		break;
	}
	return 1;
}
int CTitle::Draw()
{
	D3DVIEWPORT9 view = { 0, 0, WINDOW_WIDTH, WINDOW_WIDTH, 0.0, 1.0 };
	Sun3D.SetViewport( view );

	RECT rect;
	const int h = 32;
	const int h2 = 100;
	int x=50;
	int y=220;
	switch( mode ) {
	case 0: //タイトル画面
		if( g_pTitle->otaku ) {
			SetRect(&rect,0,0,512,480);
			g_pTitleMaterial->imgTitle.DrawRect(0,0,rect);
			SetRect(&rect,0,0,128,480);
			g_pTitleMaterial->imgRight.DrawRect(512,0,rect);
		}
		else
		Sun3D.Clear( xrgb(0,0,0));

		//Start
		SetRect(&rect,0,0,104,32);
		g_pTitleMaterial->imgTitleMoji.DrawRect(x,y,rect,alpha(128+127*(title_select==0)));
		y+=h;
		
		//Extra Start
		//SetRect(&rect,0,32,216,64);
		//g_pTitleMaterial->imgTitleMoji.DrawRect(50,190+h,rect,alpha(128+127*(title_select==1)));
		
		//Practice
		SetRect(&rect,0,64,150,96);
		g_pTitleMaterial->imgTitleMoji.DrawRect(x,y,rect,alpha(128+127*(title_select==2)));
		y+=h;
		
		//Replay
		SetRect(&rect,0,96,120,136);
		g_pTitleMaterial->imgTitleMoji.DrawRect(x,y,rect,alpha(128+127*(title_select==3)));
		y+=h;
		
		//Result
		SetRect(&rect,0,136,120,168);
		g_pTitleMaterial->imgTitleMoji.DrawRect(x,y,rect,alpha(128+127*(title_select==4)));
		y+=h;

		//Music Room
		//SetRect(&rect,0,168,200,208);
		//g_pTitleMaterial->imgTitleMoji.DrawRect(50,190+5*h,rect,alpha(128+127*(title_select==5)));

		//Option
		SetRect(&rect,0,208,128,248);
		g_pTitleMaterial->imgTitleMoji.DrawRect(x,y,rect,alpha(128+127*(title_select==6)));
		y+=h;
		
		//Quit
		SetRect(&rect,128,208,256,248);
		g_pTitleMaterial->imgTitleMoji.DrawRect(x,y,rect,alpha(128+127*(title_select==7)));
		y+=h;
		break;
	case 1: //難易度選択
		SetRect(&rect,0,0,512,480);
		g_pTitleMaterial->imgSelect.DrawRect(0,0,rect);
		SetRect(&rect,256,0,384,480);
		g_pTitleMaterial->imgRight.DrawRect(512,0,rect);

		for( int i = 0; i < 4; ++i ) {
			SetRect(&rect,0,128*i,256,128*(i+1));
			g_pTitleMaterial->imgDifficult.DrawRect(50,h2*i,rect,alpha(128+127*(difficult_select==i)));
		}
		break;
	case 2: //ステージ選択
		SetRect(&rect,0,0,512,480);
		g_pTitleMaterial->imgSelect.DrawRect(0,0,rect);
		SetRect(&rect,256,0,384,480);
		g_pTitleMaterial->imgRight.DrawRect(512,0,rect);

		for(int i = 1; i < 7; i++){
			SetRect(&rect,0,32,96,64);
			g_pTitleMaterial->imgStageMoji.DrawRect(400,100+50*i,rect,alpha(128+127*(stage_select==i)));
			SetRect(&rect,i*32,0,(i+1)*32,32);
			g_pTitleMaterial->imgStageMoji.DrawRect(500,100+50*i,rect,alpha(128+127*(stage_select==i)));
		}
		for( int i = 0; i < 4; ++i ) {
			SetRect(&rect,0,128*i,256,128*(i+1));
			g_pTitleMaterial->imgDifficult.DrawRect(50,h2*i,rect,alpha(128+127*(difficult_select==i)));
		}
		break;
	case 3: //リプレイ選択
		SetRect(&rect,0,0,512,480);
		g_pTitleMaterial->imgSelect.DrawRect(0,0,rect);
		SetRect(&rect,256,0,384,480);
		g_pTitleMaterial->imgRight.DrawRect(512,0,rect);

		for(int i = 0; i < REPLAY_NUM; i++){
			if( replay_exist[i] )
				DrawNumber(100+i/10*100,80+30*(i%10),i,2,alpha(128+127*(replay_select==i)),1);
		}
		break;
	case 4: //リザルト選択
		SetRect(&rect,0,0,512,480);
		g_pTitleMaterial->imgSelect.DrawRect(0,0,rect);
		SetRect(&rect,256,0,384,480);
		g_pTitleMaterial->imgRight.DrawRect(512,0,rect);

		SetRect(&rect,0,64,100,96);
		g_pTitleMaterial->imgStageMoji.DrawRect(200,200,rect,alpha(128+127*(result_select==0)));
		
		SetRect(&rect,100,64,256,96);
		g_pTitleMaterial->imgStageMoji.DrawRect(200,250,rect,alpha(128+127*(result_select==1)));

		break;
	case 6: //オプション
		SetRect(&rect,0,0,512,480);
		g_pTitleMaterial->imgSelect.DrawRect(0,0,rect);
		SetRect(&rect,256,0,384,480);
		g_pTitleMaterial->imgRight.DrawRect(512,0,rect);

		//コンフィグ
		for( int i = 0; i < 8; ++i ) {
			rect.left = 0;
			rect.top = 32*i;
			rect.bottom = 32*(i+1);
			int right[] = {80,112,112,112,128,112,144,80};
			rect.right = right[i];
			g_pTitleMaterial->imgConfig.DrawRect(50,64+32*i,rect,alpha(128+127*(option_select==i)));
		}
		//FullScrean
		SetRect(&rect,96,0,256,32);
		g_pTitleMaterial->imgConfig.DrawRect(200,64,rect,alpha(128+127*(optMode.m_now==0)));
		//Window
		SetRect(&rect,128,32,256,64);
		g_pTitleMaterial->imgConfig.DrawRect(360,64,rect,alpha(128+127*(optMode.m_now==1)));
		//16Bit
		SetRect(&rect,128,64,256,96);
		g_pTitleMaterial->imgConfig.DrawRect(200,96,rect,alpha(128+127*(optGraphic.m_now==16)));
		//32Bit
		SetRect(&rect,128,96,256,128);
		g_pTitleMaterial->imgConfig.DrawRect(360,96,rect,alpha(128+127*(optGraphic.m_now==32)));
		//BGMVol
		{
			SunEffectGauge gauge;
			SetRect( &gauge.m_rect, 200, 132, 400, 156 );
			gauge.m_point = (double)optBGMVol.m_now/100.0;
			gauge.m_downColor = xrgb(0,0,0);
			gauge.m_yoko = 1;
			gauge.Draw();

			gauge.m_rect.left += 1;
			gauge.m_rect.top += 1;
			gauge.m_rect.right -= 1;
			gauge.m_rect.bottom -= 1;
			gauge.m_downColor = xrgb(255,255,255);
			gauge.Draw();
		}
		//SEVol
		{
			SunEffectGauge gauge;
			SetRect( &gauge.m_rect, 200, 164, 400, 188 );
			gauge.m_point = (double)optSEVol.m_now/100.0;
			gauge.m_downColor = xrgb(0,0,0);
			gauge.m_yoko = 1;
			gauge.Draw();

			gauge.m_rect.left += 1;
			gauge.m_rect.top += 1;
			gauge.m_rect.right -= 1;
			gauge.m_rect.bottom -= 1;
			gauge.m_downColor = xrgb(255,255,255);
			gauge.Draw();
		}
		//Interval
		{
			SetRect(&rect,1*24,0,(1+1)*24,32);
			g_pTitleMaterial->imgKeyConfig.DrawRect(200,64+32*4,rect,alpha(128+127*(optInterval.m_now==1)));
			SetRect(&rect,2*24,0,(2+1)*24,32);
			g_pTitleMaterial->imgKeyConfig.DrawRect(250,64+32*4,rect,alpha(128+127*(optInterval.m_now==2)));
			SetRect(&rect,3*24,0,(3+1)*24,32);
			g_pTitleMaterial->imgKeyConfig.DrawRect(300,64+32*4,rect,alpha(128+127*(optInterval.m_now==3)));
		}
		break;
	case 7: //キーコンフィグ
		SetRect(&rect,0,0,512,480);
		g_pTitleMaterial->imgSelect.DrawRect(0,0,rect);
		SetRect(&rect,256,0,384,480);
		g_pTitleMaterial->imgRight.DrawRect(512,0,rect);

		//コンフィグ
		for( int i = 0; i < 7; ++i ) {
			SetRect(&rect,0,32*(i+1),150,32*(i+2));
			g_pTitleMaterial->imgKeyConfig.DrawRect(50,64+32*i,rect,alpha(128+127*(config_select==i)));
			//数字
			int inp  = optKeyConfig[i].m_now;
			SetRect(&rect,inp%10*24,0,(inp%10+1)*24,32);
			g_pTitleMaterial->imgKeyConfig.DrawRect(326,64+32*i,rect);
			SetRect(&rect,inp/10*24,0,(inp/10+1)*24,32);
			g_pTitleMaterial->imgKeyConfig.DrawRect(300,64+32*i,rect);
		}
		//Reset
		{
			rect.left = 0;
			rect.top = 32*5;
			rect.bottom = 32*(5+1);
			rect.right = 112;
			g_pTitleMaterial->imgConfig.DrawRect(50,64+32*7,rect,alpha(128+127*(config_select==7)));
		}
		//Quit
		{
			rect.left = 0;
			rect.top = 32*7;
			rect.bottom = 32*(7+1);
			rect.right = 80;
			g_pTitleMaterial->imgConfig.DrawRect(50,64+32*8,rect,alpha(128+127*(config_select==8)));
		}
		break;
	case 8: //Score
		SetRect(&rect,0,0,512,480);
		g_pTitleMaterial->imgSelect.DrawRect(0,0,rect);
		SetRect(&rect,256,0,384,480);
		g_pTitleMaterial->imgRight.DrawRect(512,0,rect);

		//Easy
		SetRect(&rect,0,128,72,160);
		g_pTitleMaterial->imgStageMoji.DrawRect(100,150,rect);

		SetRect(&rect,72,128,180,160);
		g_pTitleMaterial->imgStageMoji.DrawRect(100,200,rect);

		SetRect(&rect,180,128,256,160);
		g_pTitleMaterial->imgStageMoji.DrawRect(100,250,rect);

		SetRect(&rect,0,160,120,192);
		g_pTitleMaterial->imgStageMoji.DrawRect(100,300,rect);

		for( int i = 0; i < 4; ++i ) {
			DrawNumber(250,150+50*i,g_pRecord->data.score[i].score,8,alpha(255),1);
		}
		break;
	case 9: //Spell Card
		{
			SetRect(&rect,0,0,512,480);
			g_pTitleMaterial->imgSelect.DrawRect(0,0,rect);
			SetRect(&rect,256,0,384,480);
			g_pTitleMaterial->imgRight.DrawRect(512,0,rect);

			//スペルカード合計
			SetRect(&rect,160,32,192,64);
			g_pTitleMaterial->imgStageMoji.DrawRect(480,40,rect,alpha(255),0.75);
			DrawNumber(400,40,spell_sum,3,alpha(255),0.75);
			DrawNumber(510,40,SPELL_NUM*4,3,alpha(255),0.75);

			//難易度表示
			RECT rectdiff[] = {
				{0,128,72,160},
				{72,128,184,160},
				{184,128,256,160},
				{0,160,112,192}};
			g_pTitleMaterial->imgStageMoji.DrawRect(60,70,rectdiff[spell_difficult],alpha(255),0.75);
			//スペルカード合計（難易度別）
			SetRect(&rect,160,32,192,64);
			g_pTitleMaterial->imgStageMoji.DrawRect(480,70,rect,alpha(255),0.75);
			DrawNumber(400,70,spell_sum_diff,3,alpha(255),0.75);
			DrawNumber(510,70,SPELL_NUM,3,alpha(255),0.75);

			for( int i = 0; i < SPELL_DISPLAY; ++i ) {
				int num = spell_select*SPELL_DISPLAY+i;
				int a = 255;
				if( num < SPELL_NUM ) {
					int success = g_pRecord->data.spell[num][spell_difficult].success;
					int challenge = g_pRecord->data.spell[num][spell_difficult].challenge;
					if(success == 0)
						a = 127;

					fontSpellName[i].DrawFlame(60,110+30*i,alpha(a),argb(a,0,0,0));
					SetRect(&rect,160,32,192,64);
					g_pTitleMaterial->imgStageMoji.DrawRect(480,100+30*i,rect,alpha(a),0.75);
					DrawNumber(400,100+30*i,success,3,alpha(a),0.75);
					DrawNumber(510,100+30*i,challenge,3,alpha(a),0.75);

				}
			}
		}
		break;
	case 10: //Other Result
		break;
	}
	return 1;
}
//ダミーの処理関数
int CTitle::Process(){
	return 1;
}
//タイトル再開
int CTitle::Resume(){

	SAFE_DELETE( g_pGame );
	g_pScene = g_pTitle;

	mode = 0;

	g_pTitleMaterial = new CTitleMaterial();
	return 1;
}
//ゲーム開始
void CTitle::StartGame(){
	SAFE_DELETE( g_pGame );
	SAFE_DELETE( g_pTitleMaterial );
	int stage = (title_select==0) ? 1: stage_select;

	new CGame( title_select, difficult_select, stage);

	if( title_select == 3 ) {
		char buffer[256];
		sprintf( buffer, "replay/tbm%02d.rpy", replay_select );
		SunReplay.Play( buffer );
	}
	else {
		SunReplay.Record();
	}

	g_pPlayer	= new CMarisa();
	g_pSystem	= new CSystem();
	g_pMaterial = new CMaterial();

	g_pGame->CreateStage();
	SunReplay.Run();
}
//スペルカードフォント更新
void CTitle::UpdateSpellFont() {
	spell_sum_diff = 0;
	for( int i = 0; i < SPELL_NUM; i++ ) {
		int success = g_pRecord->data.spell[i][spell_difficult].success;
		int challenge = g_pRecord->data.spell[i][spell_difficult].challenge;
		if( success > 0 )
			spell_sum_diff ++;
	}
	for( int i = 0; i < SPELL_DISPLAY; ++i ) {
		int num = spell_select*SPELL_DISPLAY+i;
		if( num < SPELL_NUM ) {
			int success = g_pRecord->data.spell[num][spell_difficult].success;
			int challenge = g_pRecord->data.spell[num][spell_difficult].challenge;
			if( challenge != 0 )
				fontSpellName[i].Create(spell_name[num]);
			else
				fontSpellName[i].Create("？？？？？？？？");

		}
		else {
			fontSpellName[i].Release();
		}
	}
}
//数値描画 n:数値 k:桁数
int CTitle::DrawNumber(int x,int y,int n,int k,D3DCOLOR color,double scale){
	RECT rect;
	int kk = max( k, (int)log10( n ) + 1 );
	for(int i=0;i<kk;++i){
		int index = (int)(n/pow(10,kk-1-i)) % 10;
		int left = index * 32;
		if( index > 7 )
			left -= 64;
		int top = (index > 7) ? 32: 0;
		SetRect(&rect,left,top,left+32,top+32);
		g_pTitleMaterial->imgStageMoji.DrawRect(x+32*(i-(kk-k))*scale,y,rect,color,scale);
	}
	return 1;
}

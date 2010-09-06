#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//-------------------補助関数---------------------------------
int Next(){
	if( GetInputEvent( INP_B0 ))
		return 1;
	if( GetInputEvent( INP_B2 ))
		return 1;
	if( GetInputState( INP_B6 ))
		return 1;
	return 0;
}
void DrawMarisa(int x,int y,int face,int manfu,D3DCOLOR color){
	if(g_pTitle->otaku && face >= 0) {
		g_pMaterial->imgMarisaBody.DrawClip(x,y,color);
		if( face > 0 ) {
			int fx = face % 3;
			int fy = face / 3;
			RECT rect = {72*fx,64*fy,72*(fx+1),64*(fy+1)};
			g_pMaterial->imgMarisaFace.DrawRectClip(x+288,y+88,rect,color);
		}
	}
}
//-------------------会話共通---------------------------------
CTalk::CTalk(){
	text = "";
	chara = 0;
	face = 0;
	manfu = -1;
}
int CTalk::IsEnd(){
	return mt.IsEnd();
}
int CTalk::IsSuspended(){
	return mt.IsSuspended();
}
void CTalk::Resume(){
	mt.Resume();
}
void CTalk::Talk(){
	if( g_pTitle->talk ) {
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		do{
			mt.Suspend();
			int mae = (chara != 0 && chara != 2);
			D3DCOLOR color = mono(128+127*(!mae));
			int cx = - mae * 20;
			int cy = mae * 20;
			DrawMarisa(-200+cx,60+cy,face,manfu,color);
			RECT rect;
			switch( manfu ) {
			case 0://驚
				SetRect(&rect,0,0,48,64);
				break;
			case 1://困
				SetRect(&rect,48,0,128,64);
				break;
			case 2://汗
				SetRect(&rect,128,0,160,64);
				break;
			case 3://怒
				SetRect(&rect,160,0,208,64);
				break;
			default:
				break;
			}
			if( manfu != -1 )
				g_pMaterial->imgManfu.DrawRectClip(40+cx,90+cy,rect,color);
			if( g_pTitle->otaku )
				DrawEnemy();
			flame.Draw();

			D3DCOLOR c1 = (chara==1) ? ENEMY_TALK_COLOR : MARISA_TALK_COLOR;
			D3DCOLOR c2 = ENEMY_TALK_COLOR;
			font.DrawFormat(100,370,c1);
			font2.DrawFormat(100,400,c2);
			fntEnemy.DrawFormat(500-16*fntEnemy.m_str.size()/2,340,ENEMY_NAME_COLOR);
		}while( !Next() );
	}
}
//-------------------ルーミア---------------------------------
int CTalkRumia1::Start(){
	TSunFunctionCallback<CTalkRumia1> fn;
	fn.SetFunction(this,&CTalkRumia1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkRumia1::Thread(){

	chara = 0;
	face = 0;
	font.CreateList("寒いぜ寒いぜ寒すぎるぜ");
	fntEnemy.CreateList("");
	Talk();

	chara = 1;
	font.CreateList("そーなのかー");
	fntEnemy.CreateList("宵闇の妖怪 ルーミア",16);
	Talk();

	StopBGM();

	chara = 0;
	font.CreateList("ルーミアじゃないか、いつからいたんだ？\n全然気付かなかったぜ。");
	Talk();

	chara = 1;
	font.CreateList("さっき会ったけど、\nひょっとして鳥目？");
	Talk();

	chara = 0;
	face = 2;
	manfu = 1;
	font.CreateList("ひょっとしなくても鳥目だぜ。");
	Talk();

	chara = 1;
	font.CreateList("それは私のせいだけど、\n寒いのは私のせいじゃないよ");
	Talk();

	chara = 0;
	face = 1;
	manfu = 1;
	font.CreateList("日光さえ当たれば\nもうちょっとマシなんだがな");
	Talk();

	chara = 1;
	font.CreateList("日光はいやだー、\n頭がかさかさになるー");
	Talk();

	chara = 0;
	face = 2;
	manfu = 2;
	font.CreateList("肌とか、\n髪じゃないのか？");
	Talk();

	chara = 1;
	font.CreateList("むー、\nこのリボンさえ取れれば…");
	Talk();

	chara = 0;
	face = 1;
	manfu = -1;
	font.CreateList("もっといいリボンを\nプレゼントしてやろうか？");
	Talk();

	PlayBGM("tbm_rumia");
	g_lEffect3.Add( new CEffectFont("♪ウィンタークーラー",bgmFontColor,380,420) );
	return 1;
}
void CTalkRumia1::DrawEnemy(){
	D3DCOLOR c = mono(128+127*(chara==1));
	g_pStage1->imgCutin.DrawClip(270+(chara != 1)*20,120+(chara != 1)*20,c);
}
//-------------------ルーミア2---------------------------------
int CTalkRumia2::Start(){
	TSunFunctionCallback<CTalkRumia2> fn;
	fn.SetFunction(this,&CTalkRumia2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkRumia2::Thread(){
	chara = 0;
	face = 1;
	font.CreateList("月から日まで7種類あるが、どれがいい？");
	font2.CreateList("");
	Talk();

	return 1;
}
void CTalkRumia2::DrawEnemy(){
	D3DCOLOR c = mono(128+127*(chara==1));
	g_pStage1->imgCutin.DrawClip(270+(chara != 1)*20,120+(chara != 1)*20,c);
}
//-------------------リグル---------------------------------
int CTalkWriggle1::Start(){
	TSunFunctionCallback<CTalkWriggle1> fn;
	fn.SetFunction(this,&CTalkWriggle1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkWriggle1::Thread(){

	chara = 0;
	face = 0;
	enemy_face = 0;
	font.CreateList("あー\n今夜は虫が多いな");
	font2.CreateList("");
	fntEnemy.CreateList("");
	Talk();

	chara = 1;
	font.CreateList("それは、私がいるからよ！");
	fntEnemy.CreateList("蟲師 リグル",16);
	Talk();

	StopBGM();

	chara = 0;
	font.CreateList("夜の虫は光のあるところに集まる。\n私ってそんなに輝いていたか？");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("誘蛾灯かあんたは。\n虫が集まるのは光じゃなくて紫外線だし。");
	Talk();

	chara = 0;
	face = 2;
	manfu = 1;
	font.CreateList("どっかの胡散臭い妖怪と一緒にしないでくれ。\n殺虫剤のほうが強力だし。");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("殺虫剤は嫌ぁ");
	Talk();

	chara = 0;
	face = 1;
	manfu = 1;
	font.CreateList("紫外線のほうが嫌だぜ、お肌の大敵だ");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("ところで、誰だっけ？");
	Talk();

	chara = 0;
	face = 2;
	manfu = 2;
	font.CreateList("私は普通の魔法使い、霧雨魔理沙だ。\n虫頭には覚えられまい。");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("なによ、私だって痛い目に遭わされた\n相手ぐらい覚えているわよ");
	Talk();

	chara = 0;
	face = 1;
	manfu = -1;
	font.CreateList("墓穴を掘ったな？\n今から痛い目に遭わせてやろう");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("蟲に墓石は必要ない。");
	Talk();

	chara = 1;
	font.CreateList("自然に生まれ、自然に還る蟲の力、\nとくと味わわせてあげるわ！");
	Talk();

	chara = 0;
	face = 0;
	manfu = -1;
	font.CreateList("あんまし、うまそうじゃないなぁ");
	Talk();

	PlayBGM("tbm_wriggle");
	g_lEffect3.Add( new CEffectFont("♪薔薇十字探偵社",bgmFontColor,380,420) );
	return 1;
}
void CTalkWriggle1::DrawEnemy(){
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	switch( enemy_face ) {
	case 0:
		g_pStage2->imgCutin.DrawClip(360+(chara != 1)*20,100+(chara != 1)*20,c);
		break;
	case 1:
		SetRect(&rect,0,0,256,438);
		g_pStage2->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 2:
		SetRect(&rect,256,0,512,438);
		g_pStage2->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	}
}
//-------------------リグル2---------------------------------
int CTalkWriggle2::Start(){
	TSunFunctionCallback<CTalkWriggle2> fn;
	fn.SetFunction(this,&CTalkWriggle2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkWriggle2::Thread(){

	chara = 0;
	face = 0;
	manfu = -1;
	font.CreateList("殺虫剤も使い過ぎれば空に穴を開け、\n紫外線を増やす。");
	font2.CreateList("");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("学校で習わなかったか？\nまぁ習っても忘れたか、所詮虫だしな。");
	Talk();

	return 1;
}
void CTalkWriggle2::DrawEnemy(){
	D3DCOLOR c = mono(128+127*(chara==1));
	g_pStage2->imgCutin.DrawClip(360+(chara != 1)*20,100+(chara != 1)*20,c);
}

//-------------------チルノ1---------------------------------
int CTalkChirno1::Start(){
	TSunFunctionCallback<CTalkChirno1> fn;
	fn.SetFunction(this,&CTalkChirno1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChirno1::Thread(){

	chara = 0;
	face = 0;
	enemy_face = -1;
	font.CreateList("いるんだろ？\n出てこいよ。");
	Talk();

	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		D3DCOLOR color = mono(128+127*(chara==0));
		int cx = - (chara != 0) * 20;
		int cy = (chara != 0) * 20;
		DrawMarisa(-200+cx,60+cy,face,manfu,color);
		flame.Draw();
		if( !GetInputState( INP_B6 ) )
			mt.Suspend();
	}

	chara = 1;
	enemy_face = 3;
	font.CreateList("な、なんで分かったのよ……");
	fntEnemy.CreateList("氷精 チルノ",16);
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("妖精が現れるのには、前触れがあるんだぜ、\n知らないのか？");
	Talk();

	return 1;
}
void CTalkChirno1::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	SetRect(&rect,0,0,300,512);
	g_pStage3->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-128,0,512,128);
		g_pStage3->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 2:
		SetRect(&rect,512-128,128,512,256);
		g_pStage3->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 3:
		SetRect(&rect,512-128,256,512,256+128);
		g_pStage3->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	}
}

//-------------------チルノ2---------------------------------
int CTalkChirno2::Start(){
	TSunFunctionCallback<CTalkChirno2> fn;
	fn.SetFunction(this,&CTalkChirno2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChirno2::Thread(){

	chara = 0;
	face = 3;
	enemy_face = -1;
	font.CreateList("む、誰だ！");
	Talk();

	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		D3DCOLOR color = mono(128+127*(chara==0));
		int cx = - (chara != 0) * 20;
		int cy = (chara != 0) * 20;
		DrawMarisa(-200+cx,60+cy,face,manfu,color);
		flame.Draw();
		if( !GetInputState( INP_B6 ) )
			mt.Suspend();
	}

	chara = 1;
	enemy_face = 2;
	font.CreateList("さっき遭ったじゃない！");
	fntEnemy.CreateList("氷精 チルノ",16);
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("そうだったか？");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("ふん、\nこの寒さで頭がおかしくなったんじゃない？");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("ところでこの寒さは……");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("ふっふっふ");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("お前以外の誰の仕業だ？");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("むっ");
	Talk();

	chara = 0;
	face = 2;
	font.CreateList("って、知らなさそうだな\nそもそもおまえは夏以外出没禁止なんだが");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("大ガマが冬眠している今こそ、\n私が活躍するチャンスなのよ！");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("まぁいいや。");
	Talk();
	face = 1;
	font.CreateList("３×３は？");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("⑨！");
	Talk();

	StopBGM();
	PlayBGM("tbm_otenba");
	g_lEffect3.Add( new CEffectFont("♪バカとバカの境界",bgmFontColor,380,420) );

	return 1;
}
//-------------------チルノ3---------------------------------
int CTalkChirno3::Start(){
	TSunFunctionCallback<CTalkChirno3> fn;
	fn.SetFunction(this,&CTalkChirno3::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChirno3::Thread(){

	chara = 0;
	face = 4;
	enemy_face = 2;
	font.CreateList("なんだ、お前の仕業じゃないのか……");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("最初から分かってたでしょ！");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("はっはっは");
	Talk();

	return 1;
}

//-------------------ミスティア1---------------------------------
int CTalkMystia1::Start(){
	TSunFunctionCallback<CTalkMystia1> fn;
	fn.SetFunction(this,&CTalkMystia1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkMystia1::Thread(){
	chara = 0;
	face = 0;
	enemy_face = -1;
	font.CreateList("あー何だろな、この場所は");
	Talk();

	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		D3DCOLOR color = mono(128+127*(chara==0));
		int cx = - (chara != 0) * 20;
		int cy = (chara != 0) * 20;
		DrawMarisa(-200+cx,60+cy,face,manfu,color);
		flame.Draw();
		if( !GetInputState( INP_B6 ) )
			mt.Suspend();
	}

	chara = 1;
	enemy_face = 2;
	font.CreateList("ある～晴れ～た日～の事～♪");
	fntEnemy.CreateList("みみずくの夜 ミスティア",16);
	Talk();

	chara = 0;
	face = 4;
	font.CreateList("なんだ、夜雀の巣か");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("魔砲以上の～愉快な～♪");
	Talk();

	chara = 0;
	face = 3;
	manfu = 3;
	font.CreateList("歌うな！");
	Talk();

	return 1;
}
void CTalkMystia1::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	SetRect(&rect,0,0,256+32,512);
	g_pStage4->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-128,0,512,128);
		g_pStage4->imgTalk.DrawRectClip(32+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 2:
		SetRect(&rect,512-128,128,512,256);
		g_pStage4->imgTalk.DrawRectClip(32+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 3:
		SetRect(&rect,512-128,256,512,384);
		g_pStage4->imgTalk.DrawRectClip(32+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	}
}
//-------------------ミスティア2---------------------------------
int CTalkMystia2::Start(){
	TSunFunctionCallback<CTalkMystia2> fn;
	fn.SetFunction(this,&CTalkMystia2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkMystia2::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 2;
	font.CreateList("ねえ");
	fntEnemy.CreateList("みみずくの夜 ミスティア",16);
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("ん？");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("あなた、人間？");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("いいや、魔理沙だぜ");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("次からは人間をさらってきてほしいものね");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("私が人間だ。");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("黒いのに人間？");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("黒い人間だぜ");
	Talk();
	font.CreateList("といっても私を食べる気なら、\n被害は補償しないがな");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("黒い人間はどういう味がするのかしら？");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("ええと、目の前の鳥をおいしく料理する方法は…");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("目を開けても真っ暗にしてやる！");
	Talk();

	StopBGM();
	PlayBGM("tbm_mystia");
	g_lEffect3.Add( new CEffectFont("♪光を貫く闇",bgmFontColor,380,420) );

	return 1;
}
//-------------------ミスティア3---------------------------------
int CTalkMystia3::Start(){
	TSunFunctionCallback<CTalkMystia3> fn;
	fn.SetFunction(this,&CTalkMystia3::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkMystia3::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 1;
	font.CreateList("あのカラスに言われた通り、\n近づかなければ良かったわ");
	Talk();

	return 1;
}
//-------------------橙1---------------------------------
int CTalkChen1::Start(){
	TSunFunctionCallback<CTalkChen1> fn;
	fn.SetFunction(this,&CTalkChen1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChen1::Thread(){
	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		mt.Suspend();
	}
	return 1;
}
void CTalkChen1::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	SetRect(&rect,0,0,320,512);
	g_pStage5->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-96,0,512,64);
		g_pStage5->imgTalk.DrawRectClip(360+64+(chara != 1)*20,100+64+(chara != 1)*20,rect,c);
		break;
	}
}
//-------------------橙2---------------------------------
int CTalkChen2::Start(){
	TSunFunctionCallback<CTalkChen2> fn;
	fn.SetFunction(this,&CTalkChen2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChen2::Thread(){
	chara = 0;
	face = 0;
	enemy_face = -1;
	font.CreateList("あーここは、普通に暖かいな。\nこたつで丸くなりたいぜ");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("こら～！\nこんなところで何をしているの！");
	fntEnemy.CreateList("ブラックキャット 橙",16);
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("普通に暴れているだけだぜ");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("私のこたつでぬくぬくを邪魔した罰は重いわ。\n罰金百両ね");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("そいつは普通に悪いことをしたな、\nお詫びに遊んでやるよ");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("犯人が誰か、聞かないの？");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("普通にその必要は無いな。\n目の前の化猫を退治するだけだ");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("その格好で陰陽師を気取るつもり？");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("いいや、普通に魔物ハンターだ");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("人間のくせに！");
	Talk();

	PlayBGM("tbm_chen");
	return 1;
}
//-------------------橙3---------------------------------
int CTalkChen3::Start(){
	TSunFunctionCallback<CTalkChen3> fn;
	fn.SetFunction(this,&CTalkChen3::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChen3::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 1;
	font.CreateList("一体何なの？");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("私は、普通だぜ");
	Talk();
	return 1;
}

//-------------------レティ1---------------------------------
int CTalkLety1::Start(){
	TSunFunctionCallback<CTalkLety1> fn;
	fn.SetFunction(this,&CTalkLety1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkLety1::Thread(){
	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		mt.Suspend();
	}
	return 1;
}
void CTalkLety1::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	int mae = (chara != 1 && chara != 2);
	D3DCOLOR c = mono(128+127*(!mae));
	RECT rect;
	SetRect(&rect,0,0,256,512);
	g_pStage6->imgTalk.DrawRectClip(360+mae*20,100+mae*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-128,0,512,96);
		g_pStage6->imgTalk.DrawRectClip(64+360+mae*20,64+100+mae*20,rect,c);
		break;
	case 2:
		SetRect(&rect,512-128,96,512,96*2);
		g_pStage6->imgTalk.DrawRectClip(64+360+mae*20,64+100+mae*20,rect,c);
		break;
	case 3:
		SetRect(&rect,512-128,96*2,512,96*3);
		g_pStage6->imgTalk.DrawRectClip(64+360+mae*20,64+100+mae*20,rect,c);
		break;
	}
}
//-------------------レティ2---------------------------------
int CTalkLety2::Start(){
	TSunFunctionCallback<CTalkLety2> fn;
	fn.SetFunction(this,&CTalkLety2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkLety2::Thread(){
	chara = 0;
	face = 0;
	enemy_face = -1;
	font.CreateList("さあ、そろそろ黒幕においで願おうか");
	Talk();

	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		D3DCOLOR color = mono(128+127*(chara==0));
		int cx = - (chara != 0) * 20;
		int cy = (chara != 0) * 20;
		DrawMarisa(-200+cx,60+cy,face,manfu,color);
		flame.Draw();
		if( !GetInputState( INP_B6 ) )
			mt.Suspend();
	}

	chara = 1;
	enemy_face = 0;
	font.CreateList("あら、あなたはこの美しく輝くオーロラが\n黒く見えるの？");
	fntEnemy.CreateList("冬の落とし子 レティ",16);
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("黒くたって輝けるんだぜ。\n知らないのか？");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("輝くのはあなたじゃなくて、\nあなたの魔法でしょう？");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("つれないなあ。\nそんなだからこんな寒い冬にしちまうんだぜ？");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("私はいつも通り冬を運んだだけ。\n巫女が出てこないのが何よりの証拠よ");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("私は魔法使いだぜ、\n寒さをしのぐ魔法なんて夕飯前だ");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("あなた達人間は、少しばかり近視眼になっている。\n少しは妖怪を見習ったらどう？");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("妖怪は退治するものだぜ");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("妖怪は自然を司る……");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("自然に依存する人間は、\n自然のしっぺ返しを食らうことになる");
	Talk();

	chara = 2;
	enemy_face = 1;
	face = 1;
	font.CreateList("黒の魔法を食らわせてやろう、冬の妖怪");
	font2.CreateList("白き冬を味わわせてあげるわ、魔法使い");
	Talk();

	PlayBGM("tbm_lety");
	return 1;
}
//-------------------レティ3---------------------------------
int CTalkLety3::Start(){
	TSunFunctionCallback<CTalkLety3> fn;
	fn.SetFunction(this,&CTalkLety3::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkLety3::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 1;
	font.CreateList("");
	Talk();
	return 1;
}

//-------------------エンディング---------------------------------

CEnding::CEnding(){
	Init();
}
int CEnding::Init(){
	for( int i = 0; i < SERIF_NUM; i++){
		serif[i].c = NARRATION_COLOR;
		serif[i].x = 100;
		serif[i].y = 320+30*i;
		serif[i].font.Create("");
	}
	return 1;
}
void CEnding::Talk(){
	if( g_pTitle->talk ) {
		do{
			mt.Suspend();
			imgBack.Draw(0,0);
			for( int i = 0; i < SERIF_NUM; i++){
				serif[i].font.DrawFlame(serif[i].x,serif[i].y,serif[i].c);
			}
		}while( !Next() );
	}
}
int CEnding::Start(){
	TSunFunctionCallback<CEnding> fn;
	fn.SetFunction(this,&CEnding::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
void CEnding::DrawEnemy(){}
int CEnding::Thread(){
	PlayBGM("tbm_end");

//	g_pSystem->continue_dirty = 1;
	if( g_pSystem->continue_dirty == 0 ) {
		imgBack.LoadTexture("img/ending01.png");

		serif[0].font.Create("博麗神社。");
		Talk();
		serif[1].font.Create("一時は幻想郷中を埋め尽くさんとしていた吹雪が止み、");
		Talk();
		serif[2].font.Create("延期されていた忘年会も無事終わって、");
		Talk();
		serif[3].font.Create("何事も無かったかのように正月を迎えてようとしていた。");
		Talk();
		Init();
		serif[0].font.Create("霊夢「どういう桶の吹き回しかしら、それ？」");
		Talk();
		Init();

		imgBack.LoadTexture("img/ending02.png");

		serif[0].font.Create("魔理沙「目が悪いって言われたんでな、");
		Talk();
		serif[1].font.Create("      途中で香霖堂に寄って借りてきた。");
		Talk();
		serif[2].font.Create("      私が死ぬまでの間な。");
		Talk();
		serif[3].font.Create("      あいつは私より長く生きるだろうし」");
		Talk();
		Init();
		serif[0].font.Create("霊夢「あんたの場合、");
		Talk();
		serif[1].font.Create("      飽きるか壊すかするほうが早そうだけどね」");
		Talk();
		Init();

		serif[0].font.Create("霊夢「で、こいつは？」");
		Talk();

		imgBack.LoadTexture("img/ending03.png");
		
		serif[1].font.Create("魔理沙「今回の黒幕だぜ」");
		Talk();
		serif[2].font.Create("霊夢「黒幕って……何かあったの？」");
		Talk();
		serif[3].font.Create("レティ「何もないわ」");
		Talk();
		Init();

		serif[0].font.Create("魔理沙「ま、そういうことだ。");
		Talk();
		serif[1].font.Create("      っと、そろそろ時間だな」");
		Talk();
		serif[3].font.Create("『新年あけましておめでとう！』");
		Talk();
		Init();

		serif[0].font.Create("幻想郷の冬は厳しい。");
		Talk();
		serif[1].font.Create("幻想郷に棲む少女達は適度に運動をしなければ");
		Talk();
		serif[2].font.Create("体も凍り付いてしまうのかもしれない。");
		Talk();
		if( GetDifficult() != DIFF_LUNATIC )
			serif[3].font.Create("Good End                  クリアーおめでとう！");
		else
			serif[3].font.Create("Good End                  あんたはすごい！");
		Talk();
		Init();
	}else{
		imgBack.LoadTexture("img/ending10.png");
		serif[0].font.Create("魔法の森にて。");
		Talk();
		serif[1].font.Create("魔理沙「あーあ、疲れたぜ。");
		Talk();
		serif[2].font.Create("      大体なんだ、この寒さは？");
		Talk();
		serif[3].font.Create("      まるで真冬のようじゃないか」");
		Talk();
		Init();

		serif[0].font.Create("はっきり言って真冬である。");
		Talk();
		serif[1].font.Create("幻想郷の冬は厳しいのだ。");
		Talk();

		serif[3].font.Create("Bad End          コンティニューせずにクリアを目指そう！");
		Talk();
		Init();
	}
	imgBack.LoadTexture("img/thend.png");
	if( g_pTitle->talk ) {
		for(int i = 0; i < 255; i++ ){
			mt.Suspend();
			imgBack.Draw(0,0,mono(i));
		}
	}
	Talk();
	if( g_pGame->mode == 0 || g_pGame->mode == 2 )
		g_pSystem->SaveReplay();
	else {
		g_pTitle->Resume();
		return 0;
	}
	return 1;
}

//-------------------チルノ4---------------------------------
int CTalkChirno4::Start(){
	TSunFunctionCallback<CTalkChirno4> fn;
	fn.SetFunction(this,&CTalkChirno4::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChirno4::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 1;
	font.CreateList("ここは通さないわ！");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("まだいたのか？\nずいぶん北まで来たと思ったが、案外近いんだな");
	Talk();

	return 1;
}
void CTalkChirno4::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	SetRect(&rect,0,0,300,512);
	g_pChirno2->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-128,0,512,128);
		g_pChirno2->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 2:
		SetRect(&rect,512-128,128,512,256);
		g_pChirno2->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 3:
		SetRect(&rect,512-128,256,512,256+128);
		g_pChirno2->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	}
}
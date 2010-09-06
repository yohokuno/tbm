#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

CMarisa *g_pPlayer = NULL;

const int laser_volume = -1000;
const int max_bomb = 250;
const int max_shot = 20;
const int max_cutin = 60;
const int speed_fast = 5;
const int speed_slow = 2;
const int max_renda = 10;
const int suiyose_x = 400;
const int restart_time = 60;

CMarisa::CMarisa():
	imgMarisa("img/marisa.png"),
	imgHakka("img/hakka.png"),
	imgAtari("img/atari.png"),
	font("恋符「マスタースパーク」"),
	sndLaser("snd/noise.wav"),
	sndShot("snd/shot.wav"),
	sndDamage("snd/damage.wav"),
	sndGraze("snd/graze.wav")
{
		
	SunSprite::SetOffset(0,0);
	sndLaser.SetVolume( laser_volume );
	sndShot.SetVolume( -2000 );
	sndDamage.SetVolume( -500 );
	sndGraze.SetVolume( 0 );
	
	max_kurai = 30 - 8 * GetDifficult();
	dec_kurai = 3;

	x=40;
	y=240-32;
	count=0;

	state = 0;
	atack = 0;
	slow = 0;
	muteki = 0;
	dcount = 0;
	kurai = max_kurai;
	renda = 0;
	forbit = 1;

	laser1.m_outColor = 0;
	laser1.m_lastx = GAME_RIGHT;
	laser2.m_lastx = GAME_RIGHT;
	laser3.m_lastx = GAME_RIGHT;
	laser3.m_num = 30;

}
CMarisa::~CMarisa()
{
	SunSprite::SetOffset(0,0);
}
//移動・ショット・ボム周り
int CMarisa::Run()
{
	//状態で場合わけ
	switch( state ) {
	case 0: //通常状態
		//移動
		this->Move();

		if( forbit == 0 ) {
			//攻撃
			this->Attack();

			//ボム
			this->Bomb();
		}
		else if( count == 10 )
			forbit = 0;

		break;
	case 1: //ボム中
		//移動
		this->Move();

		//ランダムに画面を揺らす
		rx = rand(-5,5);
		ry = rand(-5,5);
		SunSprite::SetOffset(rx,ry);

		//エフェクト＆当たり判定用変数
		si = sin( PI * (max_bomb - dcount - 1) / max_bomb );

		//効果音の音量を揺らす
		sndLaser.SetVolume(laser_volume + 1000*si);

		//エフェクトクラスのメンバはここで設定しないとリプレイがずれる
		{
			double lx = GetX() + rx;
			double ly = GetY() + ry;
			laser3.m_x = lx;
			laser3.m_y = ly;
			laser3.m_a = 25.0*si;
			laser3.m_color = mono(255*si);

			laser1.m_x = lx;
			laser1.m_y = ly;
			laser1.m_ina = 0;
			laser1.m_outa = 20;
			laser1.m_inColor = alpha(200*si);

			laser2.m_x = lx;
			laser2.m_y = ly;
			laser2.m_a = 23;
			laser2.m_color = alpha(60*si);
		}

		if( dcount == 0 ) { //終了
			state = 0;
			slow = 0;
			atack = 0;
			dcount = 0;
			sndLaser.Stop();
			SunSprite::SetOffset(0,0);
		}
		break;
	case 2: //喰らいボム受付中
		//喰らいボム
		if( !this->Bomb() ){
			kurai -= dec_kurai;
		}

		//時間切れ
		if( dcount == 0 ) {
			this->Damage2();
		}
		break;
	case 3: //リスタート中
		if( dcount == 0 ) {
			dcount = 0;
			state = 0;
			muteki = 240;
		}
		break;
	}
	if( dcount > 0 )
		dcount --;
	if( muteki > 0 )
		muteki --;
	count++;

	return 1;
}
//移動処理
int CMarisa::Move()
{
	double v = 0;
	if( state == 1 ) //ボム
		v = 1.0;
	else if( slow == 0) {//高速
		v = speed_fast;
		if( GetInputState(INP_B3) )
			slow = 1;
		if( GetInputState(INP_B0) )
			renda ++;
		else
			renda = 0;
		if( renda == max_renda ) {
			slow = 1;
			renda = 0;
		}
	}
	else {//低速
		v = speed_slow;
		if( !GetInputState(INP_B3) && !GetInputState(INP_B0) ){
			slow = 0;
			renda = 0;
		}
	}
	//斜め移動
	if((GetInputState(INP_RIGHT) || GetInputState(INP_LEFT))
		&& ( GetInputState(INP_DOWN) || GetInputState(INP_UP))) {
		v = v/sqrt(2);
	}
	double px = x;
	double py = y;
	x += v * (GetInputState(INP_RIGHT) - GetInputState(INP_LEFT));
	y += v * (GetInputState(INP_DOWN) - GetInputState(INP_UP));

	//画面外のときは戻す
	if( GetX() < GAME_LEFT + MARISA_CLIP ||
		GetX() > GAME_RIGHT - MARISA_CLIP )
		x = px;
	if( GetY() < GAME_TOP + MARISA_CLIP ||
		GetY() > GAME_BOTTOM - MARISA_CLIP )
		y = py;

	//画面右でアイテム回収
	if( GetX() > suiyose_x || state == 1)
		ItemSuiyose();
	return 1;
}
//攻撃処理
int CMarisa::Attack()
{
	//攻撃状態で場合わけ
	switch( atack ) {
	case 0: //なし
		//ショット
		if( GetInputState(INP_B0) || GetInputState(INP_B2)) {
			atack = 1;
			dcount = 0;
		}
		break;
	case 1: //ショット
		if( GetInputHanasi(INP_B0) || GetInputHanasi(INP_B2)) {
			dcount = max_shot;
		}
		if( dcount == 1 ) {
			atack = 0;
			dcount = 0;
		}
		//サブショット
		int c = 8 - ( g_pSystem->life == 0 ) - ( g_pSystem->bomb == 0 );
		if( count % c == 2 ) {
			const double m = 8 + 10 * g_pSystem->power / 1000;
			sndShot.Play(0);
			for( int i=0; i<m; ++i) {
				for( int j = 0; j < 2; ++j ) {
					double dy = j ? -1 : 1;
					double da = (i*2-m+1);
					g_lShot.Add(new CSubShot( GetX(), GetY()+8.0*dy+4.0*da, 2.0*da));
				}
			}
		}
		//メインショット
		if( slow == 0 ) {
			if( count%3 == 1 ) {
				g_lShot.Add(new CMainShot(GetX(),GetY()));
			}
		}
		break;
	}
	return 1;
}
//ボム
int CMarisa::Bomb()
{
	//ボム発動
	if( GetInputEvent(INP_B1) && g_pSystem->bomb > 0){
		state = 1;
		muteki = 0;
		dcount = max_bomb;
		g_pSystem->bomb--;
		sndLaser.Play(DSBPLAY_LOOPING);
		return 0;
	}
	return 1;
}
//描画周り
int CMarisa::Draw()
{
	if( state == 0 ) {
		int a = 0;
		//低速エフェクト
		if( slow == 1 ) {
			Sun3D.SetBlendingType(BLEND_BRILLIANT);
			imgHakka.Draw(GetX()-64,GetY()-64,alpha(255),1,(double)count/30);
			imgHakka.Draw(GetX()-64,GetY()-64,alpha(255),1,(double)count/30);
			a = 200;
			//レーザー
			if( atack == 1) {
				Sun3D.SetBlendingType(BLEND_ADD);
				SunEffectLineLaser line;
				line.m_sx = GetX();
				line.m_sy = GetY();
				line.m_ex = GAME_RIGHT;
				line.m_ey = GetY();
				line.m_iw = 0;
				line.m_ow = 10;
				line.m_inColor = argb( a, 255, 255, 255 );
				line.m_outColor = argb( a/2, 0, 100, 255 );
				line.Draw();
			}
			Sun3D.SetBlendingType(BLEND_NORMAL);
		}
	}
	if( state != 3 ) {
		//魔理沙
		if( g_pLety==NULL || g_pLety->freeze == 0 ) {
			int index = (count/7) % 4;
			D3DCOLOR color = COLOR_WHITE;
			if( muteki % 10 > 5  )
				color = alpha(125);
			RECT rect={index*64,0,(index+1)*64,64};
			imgMarisa.DrawRect(x,y,rect,color);
			if( slow == 1 || state == 1) {
				Sun3D.SetBlendingType(BLEND_NORMAL);
				imgAtari.Draw( GetX() - 4, GetY() - 4 );
			}
		} else {
			g_pMaterial->imgMarisaFreeze.Draw(x,y);
		}
	}
	return 1;
}
//描画2（マスタースパーク・喰らいボムエフェクト）
int CMarisa::Draw2()
{
	//マスタースパーク
	if( state == 1 ){
		/*//暗転
		SunEffectRect effect;
		SetRect(&effect.m_rect,GAME_LEFT,GAME_TOP,GAME_RIGHT,GAME_BOTTOM);
		effect.m_color = argb(128,0,0,0);
		effect.Draw();*/

		//内殻
		Sun3D.SetBlendingType(BLEND_ADD);
		laser1.Draw();

		//中殻
		laser2.Draw();

		//カットイン
		if( dcount > max_bomb - max_cutin ) {
			RECT rect;
			int c = max_bomb - dcount;
			SetRect(&rect,128+8,0,512-8,384);
			g_pMaterial->imgCutin.DrawRect(320-184,120*(max_cutin-c)/max_cutin,rect,alpha(255*sin(PI*(c-1)/max_cutin)));
			SetRect(&rect,128+8,384,320,512);
			g_pMaterial->imgCutin.DrawRect(20,140+120*(max_cutin-c)/max_cutin,rect,alpha(255*sin(PI*(c-1)/max_cutin)));
			SetRect(&rect,320,384,512-8,512);
			g_pMaterial->imgCutin.DrawRect(640-40-184,140+120*(max_cutin-c)/max_cutin,rect,alpha(255*sin(PI*(c-1)/max_cutin)));
		}
		//外殻
		Sun3D.SetBlendingType(BLEND_BRILLIANT);
		laser3.Draw();

		//八卦炉
		imgHakka.Draw(GetX()-64,GetY()-64,alpha(255),1.0+1.0*si,(double)count/10);
		imgHakka.Draw(GetX()-64,GetY()-64,alpha(255),1.0+1.0*si,(double)count/10);
		Sun3D.SetBlendingType(BLEND_NORMAL);
		//フォント
		font.DrawFormat(55,55,xrgb(250,30,140));
	}
	if( state == 2 ) {
		g_pMaterial->imgHolic.Draw(GetX()-256,GetY()-256,alpha(255),1.0*dcount/max_kurai);
	}
	return 1;
}
//マスタースパークの当たり判定（戻り値：ダメージ）
double CMarisa::GetMasterHantei(int x,int y)
{
	double damage = 0;
	//ボム中ではない
	if( state != 1) {
		return 0;
	}
/*	//最内殻
	if( x - laser1.m_x >
		(laser1.m_lastx - laser1.m_x)/pow2(15*laser1.m_outa)*pow2(y-laser1.m_y)){
		damage += si*3.0;
	}
*/	//最外殻
	if( x - laser3.m_x >
		(laser3.m_lastx - laser3.m_x)/pow2(15*laser3.m_a)*pow2(y-laser3.m_y)){
		damage += 3;
	}
	//八卦円
	if( pow2( x - GetX() ) + pow2( y - GetY() ) < pow2( (1+si)*64 ) ) {
		damage += 1;
	}
	return damage;
}
//マスタースパークの弾消し判定
int CMarisa::GetMasterHantei2(int x,int y)
{
	//ボム中ではない
	if( state != 1) {
		return 0;
	}
	//最外殻
	if( x - laser3.m_x >
		(laser3.m_lastx - laser3.m_x)/pow2(15*laser3.m_a)*pow2(y-laser3.m_y)){
		return 1;
	}
	//八卦円
	if( pow2( x - GetX() ) + pow2( y - GetY() ) < pow2( (1+si)*64 ) ) {
		return 1;
	}
	return 0;
}
//無敵判定
int CMarisa::GetMuteki(){
	if( state ==0 && muteki == 0 )
		return 0;
	return 1;
}
//レーザーの当たり判定
int CMarisa::GetLaserHantei(RECT *pRect){
	if( state == 0 && slow == 1 && atack == 1) {
		SetRect(pRect,GetX(),GetY()-10,GAME_RIGHT,GetY()+10);
		return 1;
	}
	return 0;
}
//ダメージ
void CMarisa::Damage(){
	//効果音
	sndDamage.Play(0);
	//内部処理(喰らいボム受付中へ）
	dcount = kurai;
	state = 2;
}
//真ダメージ（喰らいボム失敗）
void CMarisa::Damage2() {
	//ダメージ
	g_pSystem->Damage();
	if( g_pBoss != NULL )
		g_pBoss->dirty = 1;
	//内部処理（リスタートへ）
	dcount = restart_time;
	state = 3;
	kurai = max_kurai;
	atack=0;
	slow=0;
}
void CMarisa::Stop(){
	SunSprite::SetOffset(0,0);
	sndLaser.SetVolume(DSBVOLUME_MIN);
}
double CMarisa::GetX(){
	return x + 44;
}
double CMarisa::GetY(){
	return y + 40;
}
void CMarisa::GetItem(int type){
	double bairitu = 1.0 + (x>suiyose_x?suiyose_x:x) /suiyose_x;
	switch(type){
	case 0://星
		g_pSystem->AddScore(SCORE_ITEMTAMA);
		break;
	case 1://パワーアップ
		g_pSystem->AddPower(10);
		break;
	case 2://点
		g_pSystem->AddScore(SCORE_ITEM1*bairitu);
		g_pSystem->item ++;
		break;
	case 3://パワーアップ(大）
		g_pSystem->AddPower(30);
		break;
	case 4://ボム
		g_pSystem->AddBomb(1);
		break;
	case 5://フルパワー
		g_pSystem->AddPower(1000);
		break;
	case 6://１UP
		g_pMaterial->sndLife.Play(0);
		g_pSystem->AddLife(1);
		break;
	case ITEM_SCORE2: //スコア（大）
		g_pSystem->AddScore(SCORE_ITEM2*bairitu);
		g_pSystem->item ++;
		break;
	case ITEM_SCORE0: //スコア（小）
		g_pSystem->AddScore(SCORE_ITEM0);
		g_pSystem->item ++;
		break;
	}
}

//グレイズ
int CMarisa::Graze(){
	sndGraze.Play(0);
	g_pSystem->AddGraze(1);
	g_pSystem->AddScore(SCORE_GRAZE);
	return 1;
}
//当たり判定のサイズ
double CMarisa::GetSize(){
	return 2.0;
}
//グレイズのサイズ
double CMarisa::GetGrazeSize() {
	return 30.0;
}
//会話ON
void CMarisa::Talk(){
	forbit = 1;
	atack = 0;
}
//会話OFF
void CMarisa::Resume(){
	forbit = 0;
}
//リセット
void CMarisa::ResetState(){
	ResetPosition();

	count=0;
	state = 0;
	atack = 0;
	slow = 0;
	muteki = 0;
	dcount = 0;
	kurai = max_kurai;
	renda = 0;
	forbit = 0;
	sndLaser.Stop();
	SunSprite::SetOffset(0,0);
}
//位置初期化
void CMarisa::ResetPosition(){
	x=40;
	y=240-32;
}
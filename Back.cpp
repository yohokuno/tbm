#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

CBack *g_pBack = NULL;


//-------------------ステージ背景共通---------------------------------

CBackStageBase::CBackStageBase(){
	count = 0;
}

//-------------------ステージ1---------------------------------


CBackStage1::CBackStage1() {
	const float Start = 100.0f;
	const float End   = 300.0f;

	//フォグ設定
	Sun3D.SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
	Sun3D.SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	Sun3D.SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));

	TSunFunctionCallback<CBackStage1> fn;
	fn.SetFunction(this,&CBackStage1::Thread);
	mt.Start(&fn,0x8000);
}
int CBackStage1::Run(){
	count += 1;
	mt.Resume();
	if( count > 300 )
		x += 10;
	return 1;
}
//床のサイズ
const int tw = 400;
const int th = 400;
const int tho = 0;
//テクスチャサイズ
const int ts = 200;
//背景色
const int a = 100;
const int r = 50;
const int g = 10;
const int b = 90;

const D3DCOLOR color = D3DCOLOR_ARGB(a,r,g,b);
const D3DCOLORVALUE fcolor = {(float)r/255,(float)g/255,(float)b/255,(float)a/255};

int CBackStage1::Draw(){
	//クリア
	Sun3D.Clear(color);

	//ビュー行列
	D3DXVECTOR3 eye( x, y, z );		//視点
	D3DXVECTOR3 at( x, ay, az );	//注視点
	D3DXVECTOR3 up( 0, 1, 0 );		//上方向
	D3DXMATRIX	view;
	D3DXMatrixLookAtLH( &view, &eye, &at, &up );
	Sun3D.SetTransform( D3DTS_VIEW, &view );

	//フォグON
	Sun3D.SetRenderState(D3DRS_FOGCOLOR,color);
	Sun3D.SetRenderState(D3DRS_FOGENABLE,true);

	//床（３Ｄ）
	SunSprite3D sprite3D(&g_pStage1->imgYuki);
	RECT rect = {tho,0,tho+th,tw};
	sprite3D.CreateRectZX( rect, -100 );
	sprite3D.SetRepeat( th/ts, tw/ts );
	sprite3D.Move( ( ((int)x-tw/2) / tw ) * tw, 0, 0 );
	sprite3D.Draw();
	sprite3D.Move( tw, 0, 0 );
//	sprite3D.SetColor(alpha(127));
	sprite3D.Draw();

	//フォグOFF
	Sun3D.SetRenderState(D3DRS_FOGENABLE,false);

	//雲
	Sun3D.SetBlendingType( BLEND_ADD );
	g_pStage1->imgYuki.DrawClip( (-30*count) % 512, 0, alpha(50) );
	g_pStage1->imgYuki.DrawClip( (-30*count) % 512 + 512, 0, alpha(50) );
	g_pStage1->imgYuki.DrawClip( (-30*count) % 512 + 1024, 0, alpha(50) );
	Sun3D.SetBlendingType( BLEND_NORMAL );

	return 1;
}

int CBackStage1::Thread(){
	x = 0;
	y = 300;
	z = 100;
	ax = x;
	ay = -10;
	az = 100;
	for( int i = 0; i < 300; i++ ){
		x += 10;
		y += -1;
		z += -160.0/300;
		mt.Suspend();
	}
	y = 0;
	z = -60;
/*	while(1){
		x += 10;
		mt.Suspend();
	}
*/	return 1;
}

//-------------------ルーミアスペル---------------------------------
CBackRumia::CBackRumia() {
	count = 0;
}

int CBackRumia::Run() {
	count++;
	return 1;
}

int CBackRumia::Draw() {
	int x = ( count * 5 ) % 512;
	RECT rect = {0,0,512,480};
	//int a = 255 - 200 * (g_pPlayer->state==1);
	int a = 255;

	g_pStage1->imgBack1.DrawRect( x - 512, 0, rect, alpha(a) );
	g_pStage1->imgBack1.DrawRect( x      , 0, rect, alpha(a) );
	g_pStage1->imgBack1.DrawRect( x + 512, 0, rect, alpha(a) );

	Sun3D.SetBlendingType( BLEND_ADD );
	g_pStage1->imgBack2.DrawRect( 0, 0, rect, alpha(a) );
	SetRect(&rect,0,0,128,480);
	g_pStage1->imgBack2.DrawRect( 512, 0, rect, alpha(a) );
	Sun3D.SetBlendingType( BLEND_NORMAL );
	return 1;
}

//-------------------ステージ2---------------------------------

CBackStage2::CBackStage2():
imgMori2("img/mori2.png"),
imgTree("img/tree.png"),
imgGrass("img/grass.png")
{
	const float Start = 500.0f;
	const float End   = 700.0f;
	//フォグ設定
	Sun3D.SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
	Sun3D.SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	Sun3D.SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));

	TSunFunctionCallback<CBackStage2> fn;
	fn.SetFunction(this,&CBackStage2::Thread);
	mt.Start(&fn,0x8000);
}
int CBackStage2::Run(){
	count += 1;
	mt.Resume();
	if( count > 300 )
		x += 5;
	return 1;
}
int CBackStage2::Draw(){
	D3DCOLOR color;
	color = xrgb(0,50,5);
	Sun3D.Clear( color );

	//ビュー行列
	D3DXVECTOR3 eye( x, y, z );	//視点
	D3DXVECTOR3 at( x, ay, az );		//注視点
	D3DXVECTOR3 up( 0, 1, 0 );		//上方向
	D3DXMATRIX	view;
	D3DXMatrixLookAtLH( &view, &eye, &at, &up );
	Sun3D.SetTransform( D3DTS_VIEW, &view );

	//フォグON
	color = xrgb(0,20,10);
	Sun3D.SetRenderState(D3DRS_FOGCOLOR,color);
	Sun3D.SetRenderState(D3DRS_FOGENABLE,true);

	//床
	SunSprite3D sprite3D(&imgMori2);
	RECT rect = {0,-512,512,512};
	sprite3D.CreateRectZX( rect, -100 );
	sprite3D.SetRepeat( 1, 1 );
	sprite3D.Move( ( ((int)x-512/2) / 1024 ) * 1024, 0, 0 );
	sprite3D.Draw();
	sprite3D.Move( 1024, 0, 0 );
	sprite3D.Draw();

	//木々
	SetRect(&rect,0,512-140,512,-140);
	sprite3D.SetTexture(&imgTree);
	sprite3D.CreateRectXY( rect, 700 );
	sprite3D.SetRepeat( 1, 1 );
	sprite3D.Move( ( ((int)x-256) / 512 ) * 512, 0, 0 );
	sprite3D.Draw();
	sprite3D.Move( 512, 0, 0 );
	sprite3D.Draw();

	//草
	SunSprite3D s(&imgMori2);
	SetRect(&rect,0,256-200,256,-200);
	s.SetTexture(&imgGrass);
	s.CreateRectXY( rect, 190+256 );
	s.SetRepeat( 1, 1 );
	s.Move( ( (int)x / 256 - 1) * 256, 0, 0 );
	s.Draw();
	s.Move( 256, 0, 0 );
	s.Draw();
	s.Move( 256, 0, 0 );
	s.Draw();

	//木々（手前）
	SetRect(&rect,0,512-220,512,-220);
	sprite3D.SetTexture(&imgTree);
	sprite3D.CreateRectXY( rect, 320 );
	sprite3D.SetRepeat( 1, 1 );
	sprite3D.Move( ( ((int)x-256) / 512 ) * 512, 0, 0 );
	sprite3D.Draw();
	sprite3D.Move( 512, 0, 0 );
	sprite3D.Draw();

	//フォグOFF
	Sun3D.SetRenderState(D3DRS_FOGENABLE,false);

	return 1;
}

int CBackStage2::Thread(){
	x = 0;
	y = 300;
	z = -100;
	ax = x;
	ay = 10;
	az = 100;
	int i;
	for( i = 0; i < 300; i++ ){
		x += 5;
		y += -1;
		z += -200.0/300;
		ax = x;
		mt.Suspend();
	}
	y = 0;
	z = -300;
	return 1;
}
//-------------------リグルスペル---------------------------------
CBackWriggle::CBackWriggle() {
	count = 0;
}

int CBackWriggle::Run() {
	count++;
	return 1;
}

int CBackWriggle::Draw() {
	int y = ( -count * 5 ) % 512;
	RECT rect = {0,0,512,512};

	g_pStage2->imgBack1.DrawRect( 0   , y, rect );
	g_pStage2->imgBack1.DrawRect( 512 , y, rect );
	g_pStage2->imgBack1.DrawRect( 0   , y + 512, rect );
	g_pStage2->imgBack1.DrawRect( 512 , y + 512, rect );

	Sun3D.SetBlendingType( BLEND_MUL );
	g_pStage2->imgBack2.DrawRect( 60, 0, rect, alpha(255) );
	Sun3D.SetBlendingType( BLEND_NORMAL );
	return 1;
}

//-------------------ステージ3---------------------------------

CBackStage3::CBackStage3():
imgYuki("img/yuki.png")
{
	//フォグ設定
	const float Start = 500.0f;
	const float End   = 700.0f;
	Sun3D.SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
	Sun3D.SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	Sun3D.SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));

	//スレッドスタート
	TSunFunctionCallback<CBackStage3> fn;
	fn.SetFunction(this,&CBackStage3::Thread);
	mt.Start(&fn,0x8000);
}
int CBackStage3::Run(){
	count += 1;
	mt.Resume();
	if( count > 300 )
		x += 5;
	return 1;
}
int CBackStage3::Draw(){
	D3DCOLOR color = xrgb(0,30,50);
	Sun3D.Clear( color );

	//ビュー行列
	D3DXVECTOR3 eye( x, y, z );	//視点
	D3DXVECTOR3 at( x, ay, az );		//注視点
	D3DXVECTOR3 up( 0, 1, 0 );		//上方向
	D3DXMATRIX	view;
	D3DXMatrixLookAtLH( &view, &eye, &at, &up );
	Sun3D.SetTransform( D3DTS_VIEW, &view );

	//フォグON
	Sun3D.SetRenderState(D3DRS_FOGCOLOR,color);
	Sun3D.SetRenderState(D3DRS_FOGENABLE,true);

	//床
	SunSprite3D sprite3D(&imgYuki);
	RECT rect = {0,-512,512,512};
	sprite3D.CreateRectZX( rect, -100 );
	sprite3D.SetRepeat( 1, 1 );
	sprite3D.Move( ( ((int)x-512/2) / 1024 ) * 1024, 0, 0 );
	sprite3D.Draw();
	sprite3D.Move( 1024, 0, 0 );
	sprite3D.Draw();

	//フォグOFF
	Sun3D.SetRenderState(D3DRS_FOGENABLE,false);

	//雲
	Sun3D.SetBlendingType( BLEND_ADD );
	imgYuki.DrawClip( (-10*count) % 512, 0, alpha(50) );
	imgYuki.DrawClip( (-10*count) % 512 + 512, 0, alpha(50) );
	imgYuki.DrawClip( (-10*count) % 512 + 1024, 0, alpha(50) );
	Sun3D.SetBlendingType( BLEND_NORMAL );

	return 1;
}
int CBackStage3::Thread(){
	x = 0;
	y = 300;
	z = -100;
	ax = x;
	ay = 10;
	az = 100;
	int i;
	for( i = 0; i < 300; i++ ){
		x += 5;
		y += -1;
		z += -200.0/300;
		ax = x;
		mt.Suspend();
	}
	y = 0;
	z = -300;
	return 1;
}

//-------------------チルノスペル---------------------------------
CBackChirno::CBackChirno() {
	count = 0;
}

int CBackChirno::Run() {
	count++;
	return 1;
}

int CBackChirno::Draw() {
	int x = ( -count * 5 ) % 512;
	int y = ( -count * 5 ) % 512;
	RECT rect;
	SetRect(&rect,GAME_LEFT,GAME_TOP,GAME_RIGHT,GAME_BOTTOM);

	g_pStage3->imgBack1.DrawRect( GAME_LEFT, GAME_TOP, rect );

	Sun3D.SetBlendingType( BLEND_MUL );
	SetRect(&rect,0,0,512,512);
	g_pStage3->imgBack2.DrawRect( x   , y, rect );
	g_pStage3->imgBack2.DrawRect( x + 512 , y, rect );
	g_pStage3->imgBack2.DrawRect( x   , y + 512, rect );
	g_pStage3->imgBack2.DrawRect( x + 512 , y + 512, rect );
	g_pStage3->imgBack2.DrawRect( x + 1024 , y, rect );
	g_pStage3->imgBack2.DrawRect( x + 1024 , y + 512, rect );
	Sun3D.SetBlendingType( BLEND_NORMAL );
	return 1;
}

//-------------------ステージ4---------------------------------

CBackStage4::CBackStage4():
imgBack("img/back4.png"),
imgMokume("img/mokume.png")
{
	//フォグ設定
	const float Start = 500.0f;
	const float End   = 900.0f;
	Sun3D.SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
	Sun3D.SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	Sun3D.SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
}
int CBackStage4::Run(){
	count += 1;
	return 1;
}
int CBackStage4::Draw(){
	D3DCOLOR color = xrgb(40,20,0);
	//Sun3D.Clear( color );
	RECT rect;

	x = count * 5;
	y = 0;
	z = -300;
	ax = x;
	ay = 10;
	az = 100;

	//ビュー行列
	D3DXVECTOR3 eye( x, y, z );		//視点
	D3DXVECTOR3 at( x, ay, az );	//注視点
	D3DXVECTOR3 up( 0, 1, 0 );		//上方向
	D3DXMATRIX	view;
	D3DXMatrixLookAtLH( &view, &eye, &at, &up );
	Sun3D.SetTransform( D3DTS_VIEW, &view );


	//フォグON
	Sun3D.SetRenderState(D3DRS_FOGCOLOR,color);
	Sun3D.SetRenderState(D3DRS_FOGENABLE,true);

	//壁
	SunSprite3D sprite3D(&imgBack);
	SetRect(&rect,-512,-512,512,512);
	sprite3D.SetTexture(&imgBack);
	sprite3D.CreateRectXY( rect, 512 );
	sprite3D.SetRepeat( 1, 1 );
	sprite3D.Move( ((int)x / 1024 ) * 1024, 0, 0 );
	sprite3D.Draw();
	sprite3D.Move( 1024, 0, 0 );
	sprite3D.Draw();

	//床
	sprite3D.SetTexture(&imgMokume);
	SetRect(&rect,0,-512,512,512);
	sprite3D.CreateRectZX( rect, -100 );
	sprite3D.SetRepeat( 1, 1 );
	sprite3D.Move( ( ((int)x-512/2) / 1024 ) * 1024, 0, 0 );
	sprite3D.Draw();
	sprite3D.Move( 1024, 0, 0 );
	sprite3D.Draw();

	//フォグOFF
	Sun3D.SetRenderState(D3DRS_FOGENABLE,false);
	return 1;
}

//-------------------ミスティアスペル---------------------------------
CBackMystia::CBackMystia() {
	count = 0;
}

int CBackMystia::Run() {
	count++;
	return 1;
}

int CBackMystia::Draw() {
	int x = ( count * 5 ) % 512;
	RECT rect;

	SetRect(&rect,0,0,512,480);
	g_pStage4->imgBack2.DrawRect( 0, 0, rect );
	SetRect(&rect,0,0,640-512,512);
	g_pStage4->imgBack2.DrawRect( 512, 0, rect );

	Sun3D.SetBlendingType( BLEND_MAX );
	g_pStage4->imgBack1.Draw( x, 0 );
	g_pStage4->imgBack1.Draw( x+512, 0 );
	g_pStage4->imgBack1.Draw( x-512, 0 );
	Sun3D.SetBlendingType( BLEND_NORMAL );
	return 1;
}

//-------------------ステージ5---------------------------------

CBackStage5::CBackStage5():
imgBack("img/tatami.png"),
imgSyoji("img/syoji.png")
{
	//フォグ設定
	const float Start = 500.0f;
	const float End   = 900.0f;
	Sun3D.SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
	Sun3D.SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	Sun3D.SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
}
int CBackStage5::Run(){
	count += 1;
	return 1;
}
int CBackStage5::Draw(){
	D3DCOLOR color = xrgb(60,50,0);
	//Sun3D.Clear( color );
	RECT rect;

	x = count * 5;
	y = 0;
	z = -300;
	ax = x;
	ay = 10;
	az = 100;

	//ビュー行列
	D3DXVECTOR3 eye( x, y, z );		//視点
	D3DXVECTOR3 at( x, ay, az );	//注視点
	D3DXVECTOR3 up( 0, 1, 0 );		//上方向
	D3DXMATRIX	view;
	D3DXMatrixLookAtLH( &view, &eye, &at, &up );
	Sun3D.SetTransform( D3DTS_VIEW, &view );


	//フォグON
	Sun3D.SetRenderState(D3DRS_FOGCOLOR,color);
	Sun3D.SetRenderState(D3DRS_FOGENABLE,true);

	//壁
	SunSprite3D sprite3D(&imgSyoji);
	SetRect(&rect,-512,-512,512,512);
	sprite3D.SetTexture(&imgSyoji);
	sprite3D.CreateRectXY( rect, 512 );
	sprite3D.SetRepeat( 1, 1 );
	sprite3D.Move( ((int)x / 1024 ) * 1024, 0, 0 );
	sprite3D.Draw();
	sprite3D.Move( 1024, 0, 0 );
	sprite3D.Draw();

	//床
	sprite3D.SetTexture(&imgBack);
	SetRect(&rect,0,-512,512,512);
	sprite3D.CreateRectZX( rect, -100 );
	sprite3D.SetRepeat( 1, 1 );
	sprite3D.Move( ( ((int)x-512/2) / 1024 ) * 1024, 0, 0 );
	sprite3D.Draw();
	sprite3D.Move( 1024, 0, 0 );
	sprite3D.Draw();

	//フォグOFF
	Sun3D.SetRenderState(D3DRS_FOGENABLE,false);
	return 1;
}

//-------------------橙スペル---------------------------------
CBackChen::CBackChen() {
	count = 0;
}

int CBackChen::Run() {
	count++;
	return 1;
}

int CBackChen::Draw() {
	int x = ( count * 5 ) % 512;
	RECT rect;
	SetRect(&rect,GAME_LEFT,GAME_TOP,GAME_RIGHT,GAME_BOTTOM);
	g_pStage5->imgBack3.DrawRect( GAME_LEFT, GAME_TOP, rect );
	Sun3D.SetBlendingType( BLEND_ADD );
	SetRect(&rect,0,0,512,512);
	g_pStage5->imgBack1.DrawRect( GAME_CENTER_X-512/2, GAME_CENTER_Y-512/2, rect, alpha(255), 1.0, -(double)count/1000);
	Sun3D.SetBlendingType( BLEND_NORMAL );
	return 1;
}
//-------------------ステージ6---------------------------------

CBackStage6::CBackStage6():
imgYuki("img/yuka.png"),
imgHyoga("img/hyoga.png")
{
	//フォグ設定
	const float Start = 500.0f;
	const float End   = 900.0f;
	Sun3D.SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
	Sun3D.SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	Sun3D.SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
}
int CBackStage6::Run(){
	count += 1;
	return 1;
}
int CBackStage6::Draw(){
	D3DCOLOR color = xrgb(20,0,70);
	Sun3D.Clear( color );
	RECT rect;

	x = 0;
	y = 0;
	z = 0;
	ax = x;
	ay = 0;
	az = 100;

	//ビュー行列
	D3DXVECTOR3 eye( x, y, z );		//視点
	D3DXVECTOR3 at( x, ay, az );	//注視点
	D3DXVECTOR3 up( 0, 1, 0 );		//上方向
	D3DXMATRIX	view;
	D3DXMatrixLookAtLH( &view, &eye, &at, &up );
	Sun3D.SetTransform( D3DTS_VIEW, &view );

	//フォグON
	Sun3D.SetRenderState(D3DRS_FOGCOLOR,color);
	Sun3D.SetRenderState(D3DRS_FOGENABLE,true);

	SunSprite3D sprite3D(&imgHyoga);

	//床
	sprite3D.SetTexture(&imgYuki);
	SetRect(&rect,0,-512,512,512);
	sprite3D.CreateRectZX( rect, -100 );
	sprite3D.SetRepeat( 1, 1 );
	sprite3D.Move( 0, 0, -(count*10) % 512 );
	sprite3D.Draw();
	sprite3D.Move( 0, 0, 512 );
	sprite3D.Draw();
	sprite3D.Move( 0, 0, 512 );
	sprite3D.Draw();

	//フォグOFF
	Sun3D.SetRenderState(D3DRS_FOGENABLE,false);

	//雲
	Sun3D.SetBlendingType( BLEND_ADD );
	D3DCOLOR c = argb(100,100,50,0);
	imgHyoga.DrawClip( (-10*count) % 512, 0, c );
	imgHyoga.DrawClip( (-10*count) % 512 + 512, 0, c );
	imgHyoga.DrawClip( (-10*count) % 512 + 1024, 0, c );

	c = argb(100,0,50,100);
	imgHyoga.DrawClip( (10*count) % 512, 0, c );
	imgHyoga.DrawClip( (10*count) % 512 - 512, 0, c );
	imgHyoga.DrawClip( (10*count) % 512 - 1024, 0, c );
	Sun3D.SetBlendingType( BLEND_NORMAL );
	return 1;
}

//-------------------レティスペル---------------------------------
CBackLety::CBackLety() {
	count = 0;
}

int CBackLety::Run() {
	return 1;
}

int CBackLety::Draw() {
	int x = ( count * 5 ) % 512;

	g_pStage6->imgBack.DrawClip( 0, 0 );
	Sun3D.SetBlendingType( BLEND_ADD );
	if( g_pLety != NULL && g_pLety->freeze )
		g_pStage6->imgEarthFreeze.Draw( GAME_CENTER_X-512/2, GAME_CENTER_Y-512/2, alpha(255), 1.0, -(double)count/2000);
	else {
		count++;
		g_pStage6->imgEarth.Draw( GAME_CENTER_X-512/2, GAME_CENTER_Y-512/2, alpha(255), 1.0, -(double)count/2000);
	}
	Sun3D.SetBlendingType( BLEND_NORMAL );
	return 1;
}
//-------------------チルノスペル2---------------------------------
CBackChirno2::CBackChirno2() {
	count = 0;
}

int CBackChirno2::Run() {
	count++;
	return 1;
}

int CBackChirno2::Draw() {
	int x = ( -count * 5 ) % 512;
	int y = ( -count * 5 ) % 512;
	RECT rect;
	SetRect(&rect,GAME_LEFT,GAME_TOP,GAME_RIGHT,GAME_BOTTOM);

	g_pChirno2->imgBack1.DrawRect( GAME_LEFT, GAME_TOP, rect );

	Sun3D.SetBlendingType( BLEND_MUL );
	SetRect(&rect,0,0,512,512);
	g_pChirno2->imgBack2.DrawRect( x   , y, rect );
	g_pChirno2->imgBack2.DrawRect( x + 512 , y, rect );
	g_pChirno2->imgBack2.DrawRect( x   , y + 512, rect );
	g_pChirno2->imgBack2.DrawRect( x + 512 , y + 512, rect );
	g_pChirno2->imgBack2.DrawRect( x + 1024 , y, rect );
	g_pChirno2->imgBack2.DrawRect( x + 1024 , y + 512, rect );
	Sun3D.SetBlendingType( BLEND_NORMAL );
	return 1;
}
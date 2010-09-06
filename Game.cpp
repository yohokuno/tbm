#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

static D3DVIEWPORT9 view1 = { GAME_LEFT, GAME_TOP, GAME_WIDTH, GAME_HEIGHT, 0.0, 1.0 };
static D3DVIEWPORT9 view2 = { 0, 0, WINDOW_WIDTH, WINDOW_WIDTH, 0.0, 1.0 };

CGame *g_pGame = NULL;
CMaterial *g_pMaterial = NULL;

CGame::CGame(int mode,int diff,int stage)
{
	this->stop = 0;
	this->mode = mode;
	this->diff = diff;
	this->stage = stage;
	g_pGame = this;
	g_pScene = g_pGame;
}
CGame::~CGame()
{
	SAFE_DELETE( g_pPlayer );
	SAFE_DELETE( g_pSystem );
	SAFE_DELETE( g_pMaterial );
	SAFE_DELETE( g_pStage );
	g_lShot.DeleteAll();
	g_lLaser.DeleteAll();
	g_lTama.DeleteAll();
	g_lItem.DeleteAll();
	g_lEnemy.DeleteAll();
	g_lEffect.DeleteAll();
	g_lEffect2.DeleteAll();
	g_lEffect3.DeleteAll();
}
//実行
int CGame::Run()
{
	if( stop == 0 ) { //実行中
		if( g_pBack != NULL)
			g_pBack->Run();
		
		if( g_pStage != NULL ) {
			if( g_pStage->Run() == 0 ) {
				SAFE_DELETE( g_pStage );
			}
		}
		if( g_pLety==NULL || g_pLety->freeze == 0 )
			g_lShot.Run();
		g_lEffect.Run();
	
		if( g_pPlayer != NULL )
		if( g_pLety==NULL || g_pLety->freeze == 0 )
			g_pPlayer->Run();
		
		g_lEnemy.Run();
		g_lItem.Run();
		g_lLaser.Run();
		g_lTama.Run();
		g_lEffect2.Run();
		g_lEffect3.Run();
	}
	if( g_pSystem != NULL )
		g_pSystem->Run(); //システムのみ停止中も実行
	return 1;
}
//描画
int CGame::Draw()
{
//	Sun3D.Clear(0);

	if( g_pBack != NULL)
		g_pBack->Draw();

	Sun3D.SetBlendingType( BLEND_ADD );
	
	g_lShot.Draw();	
	g_lEffect.Draw();
	
	Sun3D.SetBlendingType( BLEND_NORMAL );

	g_lLaser.Draw();
	g_lEnemy.Draw();
	
	if( g_pPlayer != NULL )
		g_pPlayer->Draw();
	
	g_lItem.Draw();
	g_lTama.Draw();

	if( g_pStage != NULL ) {
		g_pStage->Draw();
	}

	Sun3D.SetBlendingType( BLEND_ADD );
	g_lEffect2.Draw();
	Sun3D.SetBlendingType( BLEND_NORMAL );

	g_lEffect3.Draw();

	if( g_pPlayer != NULL )
		g_pPlayer->Draw2();

	if( g_pBoss != NULL )
		g_pBoss->Draw4();

	if( g_pSystem != NULL )
		g_pSystem->Draw();

	if( g_pBoss != NULL )
		g_pBoss->Draw2();

	return 1;
}
//描画＆動作（会話用）
int CGame::Process()
{
	if( g_pBoss != NULL )
		g_pBoss->Draw3();
	return 1;
}
//ストップする
void CGame::Stop()
{
	g_pPlayer->Stop();
	g_pStage->Stop();
	stop = 1;
}
//ストップから復帰する
void CGame::Resume()
{
	stop = 0;
	g_pSystem->state = 0;
	g_pStage->Resume();
}
//ステージ作成
void CGame::CreateStage(){
	switch(this->stage){
	case 1:
		g_pStage = new CStage1();
		break;
	case 2:
		g_pStage = new CStage2();
		break;
	case 3:
		g_pStage = new CStage3();
		break;
	case 4:
		g_pStage = new CStage4();
		break;
	case 5:
		g_pStage = new CStage5();
		break;
	case 6:
		g_pStage = new CStage6();
		break;
	}
	Sun3D.SetViewport( view1 );
}
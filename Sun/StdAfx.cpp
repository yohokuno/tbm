/**
	@file stdafx.cpp
	@brief 標準インクルードファイルを含むソース ファイル
	@bug 謎のバグ：FPSが30に落ち込む、東方も落ちる、再起動するまで直らない

	Sun.pch 生成されるプリコンパイル済ヘッダー
	stdafx.obj 生成されるプリコンパイル済タイプ情報
*/
#include "stdafx.h"

/**
	@mainpage Ｓｕｎライブラリ
	<pre>
	<h2>DirectX9.0を使った主に２Ｄゲーム向けライブラリです。</h2>

    あまり公開を前提に作っていません。
    自分が使うため（東方格芸夢を作るため）に作っています。
    ２Ｄの格闘ゲームを作るのには向いてると思います。

	<h3>モジュール</h3>
		
		Ｓｕｎはいくつかのモジュールで構成されています。
		モジュールは再利用可能な単位と思ってください。
		とはいえ、stdafxやSunIncludeは必要に応じて書き換える必要があるんですが
		・ユーティリティ
			要するにコンソールでも使えるもの。
			ほかのモジュールはすべてこのモジュールに依存する。
			SunUtility,SunConfig,SunLog,SunTask,SunExeption,SunFileがこれにあたる。
		・Direct3Dアプリケーション
			Sun,Sun3D,SunSurface,SunTexture,SunImage,SunSpriteBase,SunSprite,SunSpriteEx
			がこれに含まれます。
			
		・入力関係
		・音関係
		
    <h3>クラス説明：</h3>
		実際に使うクラスのみ解説します。
		SunSurface
			サーフェースクラス。背景など透過が必要ないものを描画。
		SunTexture
			テクスチャクラス。
		SunSpiteBase
			SunTextureに対応するスプライトクラス。
		SunImage
			分割テクスチャクラス。これ単体で描画するとき、内部でSunSpriteExを使う。
		SunSprite
			SunImageに対応するスプライトクラス。
		SunSpriteEx
			SunSpriteにレイヤーによる描画順序機能を付加。
   <h3> 変数説明：</h3>
		これらは各クラスにつき１つのグローバル変数です。
        CSun Sun;
			アプリケーション全体のはなし。
        CSunLog SunLog;
			ログファイル（log.txt)関係。
        CSunConfig2 SunConfig;
			コンフィグファイル（config.ini)関係。
        CSun3D Sun3D;
			３Ｄ関係。初期化だけ覚えてください。
	</pre>
*/
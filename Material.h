#pragma once

class CMaterial
{
public:
	SunTexture imgShot1;
	SunTexture imgShot2;
	SunTexture imgKedama;
	SunTexture imgFairy;
	SunTexture imgFairy2;
	SunTexture imgHood;
	SunTexture imgTama;
	SunTexture imgItem;
	SunTexture imgEnjin;
	SunTexture imgMarisaBody;
	SunTexture imgMarisaFace;
	SunTexture imgMarisaFreeze;
	SunTexture imgManfu;
	SunTexture imgEnemyDisplay;
	SunTexture imgGraze;
	SunTexture imgHolic;
	SunTexture imgStar;
	SunTexture imgHit;
	SunTexture imgResult;
	SunTexture	imgCutin;
	SunSoundBuffer	sndExplode;
	SunSoundBuffer	sndHit;
	SunSoundBuffer	sndItem;
	SunSoundBuffer	sndPower;
	SunSoundBuffer	sndCountDown;
	SunSoundBuffer	sndSpellStart;
	SunSoundBuffer	sndSpellEnd;
	SunSoundBuffer	sndFire;
	SunSoundBuffer	sndBell;
	SunSoundBuffer	sndLaser;
	SunSoundBuffer	sndBreak;
	SunSoundBuffer	sndBreak2;
	SunSoundBuffer	sndLife;
	SunSoundBuffer	sndMelt;
	SunSoundBuffer	sndFreeze;
	SunSoundBuffer	sndGogo;
	CMaterial() :
	imgShot1("img/shot1.png"),
	imgShot2("img/shot2.png"),
	imgKedama("img/kedama.png"),
	imgFairy("img/fairy.png"),
	imgFairy2("img/fairy2.png"),
	imgHood("img/hood.png"),
	imgTama("img/tama.png"),
	imgItem("img/item.png"),
	imgEnjin("img/enjin.png"),
	imgMarisaBody("img/marisa_body.png"),
	imgMarisaFace("img/marisa_face.png"),
	imgMarisaFreeze("img/marisa_freeze.png"),
	imgManfu("img/manfu.png"),
	imgEnemyDisplay("img/enemydisplay.png"),
	imgGraze("img/graze.png"),
	imgHolic("img/holic.png"),
	imgStar("img/star.png"),
	imgHit("img/hit.png"),
	imgResult("img/result.png"),
	imgCutin("img/cutin.png"),
	sndExplode("snd/explode.wav"),
	sndHit("snd/hit.wav"),
	sndItem("snd/item.wav"),
	sndPower("snd/power.wav"),
	sndCountDown("snd/countdown.wav"),
	sndSpellStart("snd/spellstart.wav"),
	sndSpellEnd("snd/spellend.wav"),
	sndFire("snd/fire.wav"),
	sndBell("snd/bell.wav"),
	sndLaser("snd/laser.wav"),
	sndBreak("snd/break.wav"),
	sndBreak2("snd/break2.wav"),
	sndLife("snd/life.wav"),
	sndMelt("snd/melt.wav"),
	sndFreeze("snd/freeze.wav"),
	sndGogo("snd/gogo.wav")
	{
		sndExplode.SetVolume( -1000 );
		sndHit.SetVolume( -2000 );
		sndItem.SetVolume( -2000 );
		sndPower.SetVolume( 0 );
		sndCountDown.SetVolume( 0 );
		sndSpellStart.SetVolume( 0 );
		sndSpellEnd.SetVolume( 0 );
		sndFire.SetVolume( -500 );
		sndBell.SetVolume( 0 );
		sndLaser.SetVolume( 0 );
		sndBreak.SetVolume( -1000 );
		sndBreak2.SetVolume( -2000 );
		sndLife.SetVolume( 0 );
		sndMelt.SetVolume( 0 );
		sndFreeze.SetVolume( 0 );
		sndGogo.SetVolume( 0 );
	}
	virtual ~CMaterial(){}
};

extern CMaterial *g_pMaterial;
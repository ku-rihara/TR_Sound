#pragma once
#include<Novice.h>
#include<vector>
#include"wave.h"
#include "Vector2C.h"
class SoundWave{
public:
	MONO_PCM monoPcm_;
	STEREO_PCM stereoPcm_;
	//int n, i;//
	double f0, gain;//周波数、
	const int DFTSize = 64;
	double	Wreal, Wimage;
	std::vector <double> Xreal,Ximage,ximage, xreal;
public:
	void Init();
	void Update();
	void Draw();

	void CreateWave();
	void WaveVisualize();
	void DFT();//離散フーリエ変換

};


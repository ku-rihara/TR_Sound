﻿#pragma once
#include<Novice.h>
#include"wave.h"
#include "Vector2C.h"
#include<vector>
#include<complex>
class SoundWave{
public:
	STEREO_PCM stereoPcm0_;
	STEREO_PCM stereoPcm1_;
	
	
public:
	void Init();
	void Update();
	void Draw();

	void CreateWave();
	void WaveVisualize();
	
	std::vector<std::complex<double>> DFT(const int& DFTsize, const std::vector <double>& data);
	void  FFT(std::vector<std::complex<double>>& x, const int& DFTsize,bool isReverse);
};


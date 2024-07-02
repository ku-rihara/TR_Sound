#pragma once
#include<Novice.h>
#include"wave.h"
#include "Vector2C.h"
#include<vector>
#include<complex>
class SoundWave{
public:
	MONO_PCM monoPcm_;
	STEREO_PCM stereoPcm_;
	std::vector<double>f0;
	std::vector<double>g0;
	double a0;//周波数
public:
	void Init();
	void Update();
	void Draw();

	void CreateWave();
	void WaveVisualize();
	std::vector<std::complex<double>> DFT(const int& DFTsize, const std::vector <double>& data);
	std::vector<std::complex<double>> FFT(const int& DFTsize, const std::vector <double>& data);
};


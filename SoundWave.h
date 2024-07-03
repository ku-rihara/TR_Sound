#pragma once
#include<Novice.h>
#include"wave.h"
#include "Vector2C.h"
#include<vector>
#include<complex>
class SoundWave{
public:
	MONO_PCM monoPcm0_;
	MONO_PCM monoPcm1_;
	STEREO_PCM stereoPcm_;
	
	
public:
	void Init();
	void Update();
	void Draw();

	void CreateWave();
	void WaveVisualize();
	void IIR_Filtering(const int& I,const int&J, const std::vector <double>&a, const std::vector <double>&b);
	std::vector<std::complex<double>> DFT(const int& DFTsize, const std::vector <double>& data);
	std::vector<std::complex<double>> FFT(const int& DFTsize, const std::vector <double>& data);
};


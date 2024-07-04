#pragma once
#include<Novice.h>
#include"wave.h"
#include "Vector2C.h"
#include<vector>
#include<complex>

struct VowelParams {
	double startTime;
	double endTime;
	double pitch;
	double volume;
	double F1;
	double F2;
	double F3;
	double F4;
	double B1;
	double B2;
	double B3;
	double B4;
};

class SoundWave{
public:
	MONO_PCM originalpcm_;
	MONO_PCM pcm1_;
	STEREO_PCM stereoPcm_;
	
	
public:
	void Init();
	void Update();
	void Draw();

	void CreateWave();
	void WaveVisualize();

	void IIR_FilteringSegment(const std::vector<double>& input, std::vector<double>& output, int start, int end, const std::vector<double>& a, const std::vector<double>& b, int delayI, int delayJ);
	
	std::vector<std::complex<double>> DFT(const int& DFTsize, const std::vector <double>& data);
	void  FFT(std::vector<std::complex<double>>& x, const int& DFTsize,bool isReverse);
};


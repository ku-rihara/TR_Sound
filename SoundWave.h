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

	void generate_noise_wave(MONO_PCM* monoPcm_, double f0);
	void generate_formant_noise_wave(MONO_PCM* monoPcm_, double frequency, double bandwidth, double f0);
	void create_speech_wave_with_noise(const std::string& text);

	std::vector<std::complex<double>> DFT(const int& DFTsize, const std::vector <double>& data);
	void  FFT(std::vector<std::complex<double>>& x, const int& DFTsize,bool isReverse);
};


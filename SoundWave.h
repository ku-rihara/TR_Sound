#pragma once
#include<Novice.h>
#include<vector>
#include <complex>
#include"wave.h"
#include "Vector2C.h"
class SoundWave{
public:
	MONO_PCM monoPcm_;
	STEREO_PCM stereoPcm_;
	//int n, i;//
	double f0, gain;//周波数、
	const int DFTSize = 64;
	std::vector<std::complex<double>>dft;
	
public:
	void Init();
	void Update();
	void Draw();

	void CreateWave();
	void WaveVisualize();
	/// <summary>
	/// 高速フーリエ変換
	/// </summary>
	std::vector<std::complex<double>> FFT(const int& DFTSize, const std::vector <double>& data);
	std::vector<std::complex<double>> DFT(const int& DFTsize,const std::vector <double>& data);//離散フーリエ変換

};


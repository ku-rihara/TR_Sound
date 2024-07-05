﻿#pragma once
#include<Novice.h>
#include"wave.h"
#include "Vector2C.h"
#include<vector>
#include<complex>

struct Sound {
	int handle;
	int voiceHandle;
	bool isStart;
};

class SoundWave {
public:
	STEREO_PCM originalpcm_;
	STEREO_PCM pcm1_;

	double pronunciationTime_ = 1.0;//発音時間
	Sound voice_;
	

public:
	void Init();
	void Update();
	void Draw();

	void CreateWave();
	void WaveVisualize();

	void CreateOriginalWave(double f0);
	void WaveFilter(STEREO_PCM& monoPcm_, const std::vector <double>& frequency, const double& bandwidth);
	void CreateSpeechVoice(STEREO_PCM& mosnoPcm, const std::string& text);

};
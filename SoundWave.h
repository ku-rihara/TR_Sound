#pragma once
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
	STEREO_PCM noizePcm_;
	STEREO_PCM pcm1_;

	double pronunciationTime_ = 0.5;//発音時間
	Sound voice_;
	

public:
	void Init();
	void Update();
	void Draw();

	void CreateWave();
	void WaveVisualize();

	void CreateOriginalWave(double f0);
	void WaveFilter(STEREO_PCM* monoPcm_, const double& frequency, const double& bandwidth, const double& f0);
	void CreateSpeechVoice(STEREO_PCM& mosnoPcm, const std::string& text);

};
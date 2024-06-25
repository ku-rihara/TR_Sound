#pragma once
#include<Novice.h>
#include "Vector2C.h"
#include"wave.h"
class SoundWave{
public:
	MONO_PCM monoPcm_;
	STEREO_PCM stereoPcm_;

	double f0, gain;//周波数、
	double duration_;
public:
	void Init();
	void Update();
	void Draw();

	void CreateWave();
	void WaveVisualize();

};


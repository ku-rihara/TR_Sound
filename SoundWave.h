﻿#pragma once
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
	
	
};


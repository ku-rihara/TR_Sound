#include "SoundWave.h"
#include"WindowFunction.h"
#include"IIR_Filter.h"
#include"FIR_Filter.h"

void SoundWave::Init() {

	
}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	
}

void SoundWave::WaveVisualize() {
	// 可視化のための座標取得
	int numPoint = monoPcm1_.length / 60; // 100分割
	std::vector <Vector2> wave(numPoint);
	for (int i = 0; i < numPoint; i++) {
		wave[i].x = float(i * 1280 / numPoint);
		wave[i].y = float(360 + monoPcm1_.s[i * 60] * 200); // Y座標を中央にシフトし、スケーリング
	}

	for (int i = 0; i < numPoint - 1; i++) {
		Novice::DrawLine((int)wave[i].x, (int)wave[i].y, (int)wave[i + 1].x, (int)wave[i + 1].y, WHITE);
	}
}


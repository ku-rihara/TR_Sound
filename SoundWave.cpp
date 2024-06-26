#include "SoundWave.h"


void SoundWave::Init(){
	
	const int DFTSize = 64;
	Xreal.resize(DFTSize);
	Ximage.resize(DFTSize);
	Wimage.resize(DFTSize);
	Wreal.resize(DFTSize);
	CreateWave();//波作成
	wave_write_16bit_mono(&monoPcm_, "Wavename.wav");

}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	
	
}

void SoundWave::DFT() {

}

void SoundWave::WaveVisualize() {
	// 可視化のための座標取得
	int numPoint = stereoPcm_.length / 60; // 100分割
	std::vector <Vector2> wave(numPoint);
	for (int i = 0; i < numPoint; i++) {
		wave[i].x = float(i * 1280 / numPoint);
		wave[i].y = float(360 + stereoPcm_.sL[i * 60] * 300); // Y座標を中央にシフトし、スケーリング
	}

	for (int i = 0; i < numPoint - 1; i++) {
		Novice::DrawLine((int)wave[i].x, (int)wave[i].y, (int)wave[i + 1].x, (int)wave[i + 1].y, WHITE);
	}
}


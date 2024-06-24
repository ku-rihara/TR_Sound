#include "SoundWave.h"

void SoundWave::Init(){
	
	CreateWave();//波作成
	wave_write_16bit_mono(&monoPcm_, "Box.wav");

}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	
	
}

void SoundWave::WaveVisualize() {
	// 可視化のための座標取得
	int num_points = monoPcm_.length / 100; // 100分割
	float* waveX = new float[num_points];
	float* waveY = new float[num_points];
	for (int k = 0; k < num_points; k++) {
		waveX[k] = float(k * 1280 / num_points);
		waveY[k] = float(360 + monoPcm_.s[k * 100] * 300); // Y座標を中央にシフトし、スケーリング
	}

	for (int k = 0; k < num_points - 1; k++) {
		Novice::DrawLine((int)waveX[k], (int)waveY[k], (int)waveX[k + 1], (int)waveY[k + 1], WHITE);
	}
}
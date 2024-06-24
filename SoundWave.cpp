#include "SoundWave.h"

void SoundWave::Init(){
	monoPcm_.fs = 44100;//
	monoPcm_.bits = 16;
	monoPcm_.length = monoPcm_.fs * 1;
	monoPcm_.s = (double*)calloc(monoPcm_.length, sizeof(double));

	f0 = 500;//基本周波数

	CreateWave();//波作成
	wave_write_16bit_mono(&monoPcm_, "Box.wav");

}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	//矩形波
	for (int i = 1; i <= 44; i += 2) {
		for (int n = 0; n < monoPcm_.length; n++) {
			monoPcm_.s[n] += 1.0 / i * sin(2 * M_PI * i * f0 * n / monoPcm_.fs);
		}
	}
	gain = 0.1;//ゲイン
	for (int n = 0; n < monoPcm_.length; n++) {
		monoPcm_.s[n] *= gain;
	}
	
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
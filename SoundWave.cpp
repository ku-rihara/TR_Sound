#include "SoundWave.h"
#include<vector>

void SoundWave::Init() {
	monoPcm_.fs = 44100; /* 標本化周波数 */
	monoPcm_.bits = 16; /* 量子化精度 */
	monoPcm_.length = monoPcm_.fs * 1; /* 音データの長さ */
	monoPcm_.s = (double*)calloc(monoPcm_.length, sizeof(double)); /* 音データ */

	f0 = 1.0; /* 基本周波数 */

	CreateWave();//波作成
	wave_write_16bit_mono(&monoPcm_, "White.wav");

}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {

	/* 白色雑音 */
	for (int i = 1; i <= 22050; i++)
	{
		phase = (double)rand() / RAND_MAX * 2.0 * M_PI;

		for (int n = 0; n < monoPcm_.length; n++)
		{
			monoPcm_.s[n] += sin(2.0 * M_PI * i * f0 * n / monoPcm_.fs + phase);
		}
	}

	gain = 0.001; /* ゲイン */

	for (int n = 0; n < monoPcm_.length; n++)
	{
		monoPcm_.s[n] *= gain;
	}


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
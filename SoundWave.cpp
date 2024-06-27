#include "SoundWave.h"


void SoundWave::Init(){
	
	wave_read_16bit_mono(&monoPcm_, "sine_500hz.wav");//サンプリング
	Xreal.resize(DFTSize);
	Ximage.resize(DFTSize);
	ximage.resize(DFTSize);
	xreal.resize(DFTSize);

	for (int n = 0; n < DFTSize; n++) {
		xreal[n] = monoPcm_.s[n];//x(n)の実数部
		ximage[n] = 0.0;//x(n)の虚数部	
	}
	DFT();//離散フーリエ変換

}

void SoundWave::Update() {

}

void SoundWave::Draw() {
	for (int k = 0; k < DFTSize; k++) {
		if (k <= 28) {
			Novice::ScreenPrintf(0, k * 25, "X(%d)=%2.1f+j%2.1f", k, Xreal[k], Ximage[k]);
		}
		else if (k <= 28*2) {
			Novice::ScreenPrintf(260, (k-29) * 25, "X(%d)=%2.1f+j%2.1f", k, Xreal[k], Ximage[k]);
		}
		else {
			Novice::ScreenPrintf(260*2, (k - (28*2-1)) * 25, "X(%d)=%2.1f+j%2.1f", k, Xreal[k], Ximage[k]);

		}
	}
}

void SoundWave::CreateWave() {
	
	
}

void SoundWave::DFT() {
	for (int k = 0; k < DFTSize; k++) {
		for (int n = 0; n < DFTSize; n++) {
			Wreal = cos(2.0 * M_PI * k * n / DFTSize);
			Wimage=-sin(2.0 * M_PI * k * n / DFTSize);
			Xreal[k] += Wreal * xreal[n] - Wimage * ximage[n];//x(k)の実数部
			Ximage[k] += Wreal * ximage[n] + Wimage * xreal[n];//x(k)の虚数部
		}
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


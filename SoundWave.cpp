#include "SoundWave.h"


void SoundWave::Init(){
	
	wave_read_16bit_mono(&monoPcm_, "sine_500hz.wav");//サンプリング
	dft.resize(DFTSize);
	
	dft =DFT(DFTSize,monoPcm_.s);//離散フーリエ変換

}

void SoundWave::Update() {

}

void SoundWave::Draw() {
	for (int k = 0; k < DFTSize; k++) {
		if (k <= 28) {
			Novice::ScreenPrintf(0, k * 25, "X(%d)=%2.1f+j%2.1f", k, dft[k].real(), dft[k].imag());
		}
		else if (k <= 28*2) {
			Novice::ScreenPrintf(260, (k-29) * 25, "X(%d)=%2.1f+j%2.1f", k, dft[k].real(), dft[k].imag());
		}
		else {
			Novice::ScreenPrintf(260*2, (k - (28*2-1)) * 25, "X(%d)=%2.1f+j%2.1f", k, dft[k].real(), dft[k].imag());

		}
	}
}

void SoundWave::CreateWave() {
	
	
}

std::vector<std::complex<double>> SoundWave::DFT(const int& DFTsize, const std::vector <double>& data) {

	std::vector<std::complex<double>>result(DFTsize, {});
	std::vector<std::complex<double>>x(DFTsize, {});
	std::complex<double>imag(0, 1);
	double	theta;

	//読み込んだ音データのコピー
	for (int n = 0; n < DFTsize; n++) {
		x[n].real(data[n]);//x(n)の実数部
		x[n].imag(0.0);//x(n)の虚数部	
	}
	

	for (int k = 0; k < DFTsize; k++) {
		for (int n = 0; n < DFTsize; n++) {
			theta = 2.0 * M_PI * k * n / DFTsize;//θ
			result[k] += x[n] * std::exp(-imag*theta);//フーリエ変換の公式
		}
	}
	return result;
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


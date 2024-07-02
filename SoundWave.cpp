﻿#include "SoundWave.h"

void SoundWave::Init(){
	
	monoPcm_.fs = 44100;/*標本化周波数*/
	monoPcm_.bits = 16;/*16バイト*/
	monoPcm_.length = int(monoPcm_.fs * 4.0);/*音データの長さ*/
	monoPcm_.s.resize(monoPcm_.length);/*音データ*/
	//
	a0 = 0.5;/*振幅*/
	f0.resize(monoPcm_.length);
	g0.resize(monoPcm_.length);
	CreateWave();//波作成
	wave_write_16bit_mono(&monoPcm_, "Wavename.wav");

}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	//周波数の時間エンベロープ
	f0[0] = 2500;
	f0[monoPcm_.length - 1] = 1500;

	//f0[n]
	for (int n = 0; n < monoPcm_.length; n++) {
		f0[n] = f0[0] + ((f0[monoPcm_.length - 1] - f0[0]) * n) / (monoPcm_.length - 1);
	}
	//g0[n]
	for (int n = 0; n < monoPcm_.length; n++) {
		g0[n] = (f0[0]*n) + ((f0[monoPcm_.length - 1] - f0[0]) * std::pow(n,2)) / (2.0*(monoPcm_.length - 1));
	}
	
	for (int n = 0; n < monoPcm_.length; n++) {
		monoPcm_.s[n] = a0 * sin(2 * M_PI * g0[n]  / monoPcm_.fs);
	}
}

void SoundWave::WaveVisualize() {
	// 可視化のための座標取得
	int numPoint = monoPcm_.length / 60; // 100分割
	std::vector <Vector2> wave(numPoint);
	for (int i = 0; i < numPoint; i++) {
		wave[i].x = float(i * 1280 / numPoint);
		wave[i].y = float(360 + monoPcm_.s[i * 60] * 200); // Y座標を中央にシフトし、スケーリング
	}

	for (int i = 0; i < numPoint - 1; i++) {
		Novice::DrawLine((int)wave[i].x, (int)wave[i].y, (int)wave[i + 1].x, (int)wave[i + 1].y, WHITE);
	}
}

//フーリエ変換
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
			result[k] += x[n] * std::exp(-imag * theta);//フーリエ変換の公式
		}
	}
	return result;
}


std::vector<std::complex<double>> SoundWave::FFT(const int& DFTsize, const std::vector <double>& data) {

	int FFTStage = int(std::log2(DFTsize));/*FFTの段階()*/

	std::vector<std::complex<double>>x(DFTsize, {});
	std::complex<double>imag(0, 1);// 虚数単位
	//値の格納用
	std::complex<double>a{};
	std::complex<double>b{};
	std::complex<double>c{};
	std::complex<double>d{};
	//読み込んだデータのコピー
	for (int n = 0; n < DFTsize; n++) {
		x[n].real(data[n]);//実数部
		x[n].imag(0.0);//虚数部
	}

	//バタフライ演算
	for (int stage = 1; stage <= FFTStage; stage++) {//FFTの段階

		for (int i = 0; i < std::pow(2, stage - 1); i++) {

			for (int j = 0; j < std::pow(2, FFTStage - stage); j++) {

				// バタフライ演算のインデックス計算
				int n = int(std::pow(2, FFTStage - stage + 1) * i + j);
				int m = int(std::pow(2, FFTStage - stage) + n);
				int r = int(std::pow(2, stage - 1) * j);
				a = x[n];
				b = x[m];
				double theta = (2.0 * M_PI * r) / DFTsize;//θ
				//演算内容を書く
				if (stage < FFTStage) {
					x[n] = a + b;
					x[m] = (a - b) * std::exp(-imag * theta);
				}
				else {//最後の段階では足し算と引き算になる
					x[n] = a + b;
					x[m] = a - b;
				}
			}
		}
	}

	//インデックスの並び変え用のテーブル作成
	std::vector<int>index(DFTsize);
	for (int stage = 1; stage <= FFTStage; stage++) {

		for (int i = 0; i < std::pow(2, stage - 1); i++) {
			index[int(std::pow(2, stage - 1) + i)] =
				int(index[i] + std::pow(2, FFTStage - stage));
		}
	}
	//インデックス並び替え
	for (int k = 0; k < DFTsize; k++) {
		if (index[k] > k) {
			d = x[index[k]];
			x[index[k]] = x[k];
			x[k] = d;
		}
	}
	return x;
}

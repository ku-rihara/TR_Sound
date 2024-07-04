#include "SoundWave.h"
#include"WindowFunction.h"
#include"IIR_Filter.h"


void SoundWave::Init() {

	wave_read_16bit_mono(&originalpcm_, "pulse_train.wav");
	pcm1_.fs = originalpcm_.fs;
	pcm1_.bits = originalpcm_.bits;
	pcm1_.length = originalpcm_.length;
	pcm1_.s.resize(pcm1_.length);

	CreateWave();//波作成
	wave_write_16bit_mono(&pcm1_, "Wavename.wav");
}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	std::vector<double>a(3);/**/
	std::vector<double>b(3);/**/

	std::vector<double>s(pcm1_.length);
	double F1 = 500.0;/*F1の周波数*/
	double F2 = 800.0;/*F2の周波数*/
	double F3 = 2500.0;/*F3の周波数*/
	double F4 = 3500.0;/*F4の周波数*/

	double B1 = 100.0;/*F1の帯域幅*/
	double B2 = 100.0;/*F2の帯域幅*/
	double B3 = 100.0;/*F3の帯域幅*/
	double B4 = 100.0;/*F4の帯域幅*/

	int delayJ = 2;/*遅延器の数*/
	int delayI = 2;/*遅延器の数*/

	IIR_resonator(F1 / originalpcm_.fs, F1 / B1, a, b);/*IIRフィルタ設計*/
	IIR_Filtering(originalpcm_.s, s, originalpcm_.length, a, b, delayI, delayJ);/*フィルタリング*/
	for (int n = 0; n < pcm1_.length; n++) {
		pcm1_.s[n] += s[n];
		s[n] = 0.0;
	}

	IIR_resonator(F2 / originalpcm_.fs, F2 / B2, a, b);/*IIRフィルタ設計*/
	IIR_Filtering(originalpcm_.s, s, originalpcm_.length, a, b, delayI, delayJ);/*フィルタリング*/
	for (int n = 0; n < pcm1_.length; n++) {
		pcm1_.s[n] += s[n];
		s[n] = 0.0;
	}

	IIR_resonator(F3 / originalpcm_.fs, F3 / B3, a, b);/*IIRフィルタ設計*/
	IIR_Filtering(originalpcm_.s, s, originalpcm_.length, a, b, delayI, delayJ);/*フィルタリング*/
	for (int n = 0; n < pcm1_.length; n++) {
		pcm1_.s[n] += s[n];
		s[n] = 0.0;
	}

	IIR_resonator(F4 / originalpcm_.fs, F4 / B4, a, b);/*IIRフィルタ設計*/
	IIR_Filtering(originalpcm_.s, s, originalpcm_.length, a, b, delayI, delayJ);/*フィルタリング*/
	for (int n = 0; n < pcm1_.length; n++) {
		pcm1_.s[n] += s[n];
		s[n] = 0.0;
	}
	/*ディエンファシス*/
	s[0] = pcm1_.s[0];
	for (int n = 1; n < pcm1_.length; n++) {
		s[n] = pcm1_.s[n] + 0.98 * s[n - 1];
	}
	for (int n = 0; n < pcm1_.length; n++) {
		pcm1_.s[n] = s[n];
	}
}

void SoundWave::WaveVisualize() {
	// 可視化のための座標取得
	int numPoint = pcm1_.length / 60; // 100分割
	std::vector <Vector2> wave(numPoint);
	int screenHeight = 720; // 画面の高さ（仮に720ピクセルとします）
	for (int i = 0; i < numPoint; i++) {
		wave[i].x = float(i * 1280 / numPoint);
		wave[i].y = float(screenHeight / 2 - pcm1_.s[i * 60] * 200); // Y座標を中央にシフトし、スケーリングし、上下反転
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


void  SoundWave::FFT(std::vector<std::complex<double>>& x, const int& DFTsize, bool isReverse) {

	int FFTStage = int(std::log2(DFTsize));/*FFTの段階()*/

	std::complex<double>imag(0, 1);// 虚数単位
	//値の格納用
	std::complex<double>a{};
	std::complex<double>b{};
	std::complex<double>c{};
	std::complex<double>d{};
	////読み込んだデータのコピー

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
					if (isReverse) {
						x[m] = (a - b) * std::exp(imag * theta);
					}
					else {
						x[m] = (a - b) * std::exp(-imag * theta);
					}
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
	/* 計算結果をNで割る */
	for (int k = 0; k < DFTsize; k++) {
		if (isReverse) {
			x[k] /= DFTsize;
		}
	}
}

void SoundWave::IIR_FilteringSegment(const std::vector<double>& input, std::vector<double>& output, int start, int end, const std::vector<double>& a, const std::vector<double>& b, int delayI, int delayJ) {
	// Implement IIR filtering for a segment of the input
	std::vector<double> x(delayI + 1, 0.0);
	std::vector<double> y(delayJ + 1, 0.0);

	for (int n = start; n < end; n++) {
		x[0] = input[n];
		y[0] = 0.0;
		for (int i = 0; i < delayI + 1; i++) {
			y[0] += b[i] * x[i];
		}
		for (int j = 1; j < delayJ + 1; j++) {
			y[0] -= a[j] * y[j];
		}
		for (int i = delayI; i > 0; i--) {
			x[i] = x[i - 1];
		}
		for (int j = delayJ; j > 0; j--) {
			y[j] = y[j - 1];
		}
		output[n] = y[0];
	}
}
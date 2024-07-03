#include "SoundWave.h"
#include"WindowFunction.h"
#include"IIR_Filter.h"
#include"FIR_Filter.h"

void SoundWave::Init() {

	wave_read_16bit_mono(&monoPcm0_, "sine_500hz_3500hz.wav");
	monoPcm1_.fs = monoPcm0_.fs;
	monoPcm1_.bits = monoPcm0_.bits;
	monoPcm1_.length = monoPcm0_.length;
	monoPcm1_.s.resize(monoPcm1_.length);

	CreateWave();//波作成
	wave_write_16bit_mono(&monoPcm1_, "Wavename.wav");
}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	double fe = 1000.0 / monoPcm0_.fs;/*エッジ周波数*/
	double delta = 1000.0 / monoPcm0_.fs;/*遷移帯域幅*/
	int delayJ = (int)(3.1 / delta + 0.5) - 1;/*遅延器の数*/
	if (delayJ % 2 == 1) {
		delayJ++;//delayJ+1の値を奇数になるようにする
	}
	std::vector<double>b;
	std::vector<double>w;

	w = HanningWindow(delayJ + 1);/*ハニング窓*/
	b = FIR_LPF(fe, delayJ, w);/*FIRフィルタの設計*/
	int L = 128;/*フレームの長さ*/
	int N = 256;/*DFTのサイズ*/
	std::vector<std::complex<double>>x(N);
	std::vector<std::complex<double>>y(N);
	std::vector<std::complex<double>>d(N);
	int numberOfFrame = monoPcm0_.length / L;/*フレームの数*/

	for (int frame = 0; frame < numberOfFrame; frame++) {
		int ofset = L * frame;//オフセット
		/*X(k)*/
		for (int n = 0; n < N; n++) {
			x[n] =0;
		}
		for (int n = 0; n < L; n++) {
			x[n].real(monoPcm0_.s[ofset + n]);
		}
		FFT(x, N,false);//高速フーリエ変換

		/*B(k)*/
		for (int m = 0; m < N; m++) {
			d[m] = 0;
		}
		for (int m = 0; m < delayJ; m++) {
			d[m].real(b[m]);
		}
		FFT(d, N,false);//高速フーリエ変換

		//フィルタリング
		for (int k = 0; k < N; k++) {

		}
	}
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
}

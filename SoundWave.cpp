#include "SoundWave.h"
#include"WindowFunction.h"
#include"IIR_Filter.h"
#include<map>
#include <string>
#include<random>

void SoundWave::Init() {
	pcm1_.fs = 44100;
	pcm1_.bits = 16;
	pcm1_.length = int(pcm1_.fs * 4.0);
	pcm1_.s.resize(pcm1_.length);

	noizePcm_.fs = 44100;
	noizePcm_.bits = 16;
	noizePcm_.length = int(pcm1_.fs * 4.0);
	noizePcm_.s.resize(noizePcm_.length);
	generate_noise_wave(100);

	CreateWave();//波作成
	wave_write_16bit_mono(&pcm1_, "Wavename.wav");
}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	std::string text = "aio"; // 発話したいテキスト
	create_speech_wave_with_noise( text);
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


void SoundWave::generate_noise_wave( double f0) {
	
	double phase;
	for (int i = 1; i <= 120; i++) {
		phase = (double)rand() / RAND_MAX * 2.0 * M_PI;
		for (int n = 0; n < noizePcm_.length; n++) {
			noizePcm_.s[n] += sin(2.0 * M_PI * i * f0 * n / noizePcm_.fs + phase);
		}
	}

	double gain = 0.001; // ゲイン
	for (int n = 0; n < noizePcm_.length; n++) {
		noizePcm_.s[n] *= gain;
	}
}

void SoundWave::generate_formant_noise_wave(MONO_PCM* monoPcm_, double frequency, double bandwidth, double f0) {
	std::vector<double> a(3), b(3);
	IIR_resonator(frequency / monoPcm_->fs, frequency / bandwidth, a, b);

	std::vector<double> filterS(monoPcm_->length, 0.0);
	IIR_Filtering(noizePcm_.s, filterS, monoPcm_->length, a, b, 2, 2); // noizePcm_ を使用してフィルタリング

	for (int n = 0; n < monoPcm_->length; n++) {
		monoPcm_->s[n] = filterS[n];
	}
	f0;
}


void SoundWave::create_speech_wave_with_noise( const std::string& text) {
	
	double duration_per_char = 1.0; // 各文字の発話時間（秒）
	double fade_factor = 0.98; // ディエンファシスの減衰率

	int total_length = int(pcm1_.fs * duration_per_char * text.size()); // 合計の長さを計算
	pcm1_.length = total_length;
	pcm1_.s.resize(total_length, 0.0);

	int current_position = 0; // 現在の位置を管理する変数

	double f0 = 100.0; // 基本周波数
	int length = int(pcm1_.length/text.size());
	MONO_PCM monoPcm_;
	monoPcm_.fs = pcm1_.fs;
	monoPcm_.bits = pcm1_.bits;
	monoPcm_.length = length;
	monoPcm_.s.resize(length);

	for (char c : text) {
		std::map<char, std::pair<double, double>> formant_map;
		formant_map['a'] = { 800.0, 100.0 };  // あ
		formant_map['i'] = { 1500.0, 100.0 }; // い
		formant_map['u'] = { 1200.0, 100.0 }; // う
		formant_map['e'] = { 2000.0, 100.0 }; // え
		formant_map['o'] = { 700.0, 100.0 };  // お


		if (formant_map.find(c) != formant_map.end()) {
			double frequency = formant_map[c].first;
			double bandwidth = formant_map[c].second;
			generate_formant_noise_wave(&monoPcm_, frequency, bandwidth, f0);

			// ディエンファシス
			std::vector<double> s(length, 0.0);
			s[0] = monoPcm_.s[0];
			for (int n = 1; n < length; n++) {
				s[n] = monoPcm_.s[n] + fade_factor * s[n - 1];
			}

			// pcm1_ の適切な位置に波形をコピー
			for (int n = 0; n < length; ++n) {
				pcm1_.s[current_position + n] += s[n];
			}

			current_position += length; // 次の文字の位置を設定
		}
	}
}
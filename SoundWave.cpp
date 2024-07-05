#include "SoundWave.h"
#include"WindowFunction.h"
#include"IIR_Filter.h"
#include<map>
#include <string>
#include<random>
#include"InputManager.h"

void SoundWave::Init() {
	pcm1_.fs = 44100;
	pcm1_.bits = 16;
	

	noizePcm_.fs = 44100;
	noizePcm_.bits = 16;
	noizePcm_.length = int(noizePcm_.fs );
	noizePcm_.s.resize(noizePcm_.length);
	CreateOriginalWave(100);

	CreateWave();//波作成
	wave_write_16bit_mono(&pcm1_, "Wavename.wav");
	////書き出した音を読み込む
	voice_.handle = Novice::LoadAudio("./Wavename.wav");
}

void SoundWave::Update() {

	if (!voice_.isStart&&InputManager::GetInstance()->GetIsPressKey(DIK_SPACE)) {
		
		voice_.isStart = true;
	}
}

void SoundWave::Draw() {

	if (voice_.isStart) {
		Novice::PlayAudio(voice_.handle, false, 0.1f);
		voice_.isStart = false;
	}
}

void SoundWave::CreateWave() {
	std::string text = "oeeeeu"; // 発話したいテキスト
	CreateSpeechVoice(pcm1_,text);
}

void SoundWave::WaveVisualize() {
	// 可視化のための座標取得
	int numPoint = pcm1_.length / 200; // 100分割
	std::vector <Vector2> wave(numPoint);
	int screenHeight = 720; // 画面の高さ（仮に720ピクセルとします）
	for (int i = 0; i < numPoint; i++) {
		wave[i].x = float(i * 1280 / numPoint);
		wave[i].y = float(screenHeight / 2 - pcm1_.s[i * 200] * 200); // Y座標を中央にシフトし、スケーリングし、上下反転
	}
	for (int i = 0; i < numPoint - 1; i++) {
		Novice::DrawLine((int)wave[i].x, (int)wave[i].y, (int)wave[i + 1].x, (int)wave[i + 1].y, WHITE);
	}
}

void SoundWave::CreateOriginalWave( double f0) {
	
	double phase;
	for (int i = 1; i <= 120; i++) {
		phase = (double)rand() / RAND_MAX * 2.0 * M_PI;
		for (int n = 0; n < noizePcm_.length; n++) {
			noizePcm_.s[n] += 30*sin(2.0 * M_PI * i * f0 * n / noizePcm_.fs + phase);
		}
	}

	double gain = 0.001; // ゲイン
	for (int n = 0; n < noizePcm_.length; n++) {
		noizePcm_.s[n] *= gain;
	}
}

void SoundWave::WaveFilter(MONO_PCM* monoPcm_, const double& frequency, const double& bandwidth, const double& f0) {
	std::vector<double> a(3), b(3);
	IIR_resonator(frequency / monoPcm_->fs, frequency / bandwidth, a, b);

	std::vector<double> filterS(monoPcm_->length, 0.0);
	IIR_Filtering(noizePcm_.s, filterS, monoPcm_->length, a, b, 2, 2); // noizePcm_ を使用してフィルタリング

	for (int n = 0; n < monoPcm_->length; n++) {
		monoPcm_->s[n] = filterS[n];
	}
	f0;
}

void SoundWave::CreateSpeechVoice(MONO_PCM& mosnoPcm,  const std::string& text) {

	double fade_factor = 0.98; // ディエンファシスの減衰率

	int total_length = int(mosnoPcm.fs * pronunciationTime_ * (text.size())); // 合計の長さを計算
	mosnoPcm.length = total_length;
	mosnoPcm.s.resize(int(total_length), 0.0);

	int current_position = 0; // 現在の位置を管理する変数

	double f0 = 100.0; // 基本周波数
	int length = int(mosnoPcm.length/text.size());
	MONO_PCM copyMonoPcm_;
	copyMonoPcm_.fs = mosnoPcm.fs;
	copyMonoPcm_.bits = mosnoPcm.bits;
	copyMonoPcm_.length = length;
	copyMonoPcm_.s.resize(length);

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
			WaveFilter(&copyMonoPcm_, frequency, bandwidth, f0);

			// ディエンファシス
			std::vector<double> s(length, 0.0);
			s[0] = copyMonoPcm_.s[0];
			for (int n = 1; n < length; n++) {
				s[n] = copyMonoPcm_.s[n] + fade_factor * s[n - 1];
			}

			// pcm1_ の適切な位置に波形をコピー
			for (int n = 0; n < length; ++n) {
				mosnoPcm.s[current_position + n] += s[n];
			}
			current_position += length; // 次の文字の位置を設定
		}
	}
}
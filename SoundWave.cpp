#include "SoundWave.h"
#include"WindowFunction.h"
#include"IIR_Filter.h"
#include<map>
#include <string>
#include<random>

#include"InputManager.h"
const int SampleFs = 44100;
void SoundWave::Init() {
	pcm1_.fs = SampleFs;
	pcm1_.bits = 16;
	
	//ノイズ初期化
	originalpcm_.fs = SampleFs;
	originalpcm_.bits = 16;
	originalpcm_.length = int(originalpcm_.fs );
	originalpcm_.sR.resize(originalpcm_.length);
	originalpcm_.sL.resize(originalpcm_.length);
	CreateOriginalWave(400);

	CreateWave();//波作成
	wave_write_16bit_stereo(&pcm1_, "Wavename.wav");
	////書き出した音を読み込む
	voice_.handle = Novice::LoadAudio("./Wavename.wav");
}

void SoundWave::Update() {

	if (!voice_.isStart&&InputManager::GetInstance()->GetIsTriggerKey(DIK_SPACE)) {
		
		voice_.isStart = true;
	}
}

void SoundWave::Draw() { 

	if (voice_.isStart) {
		Novice::PlayAudio(voice_.handle, false, 0.7f);
		voice_.isStart = false;
	}
}

void SoundWave::CreateWave() {
	std::string text = "aiueo"; // 発話したいテキスト
	CreateSpeechVoice(pcm1_,text);
}

void SoundWave::WaveVisualize() {
	// 可視化のための座標取得
	int numPoint = pcm1_.length / 200; // 100分割
	std::vector <Vector2> waveR(numPoint);
	std::vector <Vector2> waveL(numPoint);
	int screenHeight = 720; // 画面の高さ（仮に720ピクセルとします）
	for (int i = 0; i < numPoint; i++) {
		waveR[i].x = float(i * 1280 / numPoint);
		waveR[i].y = float(screenHeight / 2 - pcm1_.sR[i * 200] * 200); // Y座標を中央にシフトし、スケーリングし、上下反転

		waveL[i].x = float(i * 1280 / numPoint);
		waveL[i].y = float(screenHeight / 2 - pcm1_.sL[i * 200] * 200); // Y座標を中央にシフトし、スケーリングし、上下反転

	}
	for (int i = 0; i < numPoint - 1; i++) {
		Novice::DrawLine((int)waveR[i].x, (int)waveR[i].y, (int)waveR[i + 1].x, (int)waveR[i + 1].y, WHITE);
		Novice::DrawLine((int)waveL[i].x, (int)waveL[i].y, (int)waveL[i + 1].x, (int)waveL[i + 1].y, WHITE);

	}
}

void SoundWave::CreateOriginalWave( double f0) {
	
	///*Rosenberg波を生成*/
	//static double t = 0;
	//double tau = 0.9;/*声門開大期*/
	//double tan2 = 0.05;/*声門閉大期*/
	//
	//t += freq / (double)SampleFs;
	//t -= floor(t);
	//if (t <= tau) {
	//	noizePcm_[]
	//}
	/*のこぎり波*/
	//for (int i = 1; i <= 44; i++) {
	//	for (int n = 0; n < noizePcm_.length; n++) {
	//		noizePcm_.sR[n] += 1.0 / i * sin(2 * M_PI * i * f0 * n / noizePcm_.fs);
	//		noizePcm_.sL[n] += 1.0 / i * sin(2 * M_PI * i * f0 * n / noizePcm_.fs);

	//	}
	//}
	//double gain = 0.1;//ゲイン
	//for (int n = 0; n < noizePcm_.length; n++) {
	//	noizePcm_.sR[n] *= gain;
	//	noizePcm_.sL[n] *= gain;
	//}

	/*ノイズ生成*/
	double phase;
	for (int i = 1; i <= 120; i++) {
		phase = (double)rand() / RAND_MAX * 2.0 * M_PI;
		for (int n = 0; n < originalpcm_.length; n++) {
			originalpcm_.sR[n] += 30*sin(2.0 * M_PI * i * f0 * n / originalpcm_.fs + phase);
			originalpcm_.sL[n] += 30 * sin(2.0 * M_PI * i * f0 * n / originalpcm_.fs + phase);
		}
	}

	double gain = 0.001; // ゲイン
	for (int n = 0; n < originalpcm_.length; n++) {
		originalpcm_.sR[n] *= gain;
		originalpcm_.sL[n] *= gain;
	}
}

void SoundWave::WaveFilter(STEREO_PCM& stereoPcm, const std::vector <double>& frequency, const double& bandwidth, const double& f0) {
	//フィルター
	std::vector<double> aR(3), bR(3);
	std::vector<double> aL(3), bL(3);
	const int delayI = 2;
	const int delayJ = 2;
	std::vector<double> filterSR(stereoPcm.length, 0.0);
	std::vector<double> filterSL(stereoPcm.length, 0.0);
	
	IIR_resonator(frequency[0] / originalpcm_.fs, frequency[0] / bandwidth, aR, bR);/*IIRフィルタ設計*/
	IIR_resonator(frequency[0] / originalpcm_.fs, frequency[0] / bandwidth, aL, bL);/*IIRフィルタ設計*/
	IIR_Filtering(originalpcm_.sR, filterSR, originalpcm_.length, aR, bR, delayI, delayJ);/*フィルタリング*/
	IIR_Filtering(originalpcm_.sL, filterSL, originalpcm_.length, aL, bL, delayI, delayJ);/*フィルタリング*/
	for (int n = 0; n < stereoPcm.length; n++) {
		stereoPcm.sR[n] += filterSR[n];
		stereoPcm.sL[n] += filterSL[n];
		filterSR[n] = 0.0;
		filterSL[n] = 0.0;
	}

	IIR_resonator(frequency[1] / originalpcm_.fs, frequency[1] / bandwidth, aR, bR);/*IIRフィルタ設計*/
	IIR_resonator(frequency[1] / originalpcm_.fs, frequency[1] / bandwidth, aL, bL);/*IIRフィルタ設計*/
	IIR_Filtering(originalpcm_.sR, filterSR, originalpcm_.length, aR, bR, delayI, delayJ);/*フィルタリング*/
	IIR_Filtering(originalpcm_.sL, filterSL, originalpcm_.length, aL, bL, delayI, delayJ);/*フィルタリング*/
	for (int n = 0; n < stereoPcm.length; n++) {
		stereoPcm.sR[n] += filterSR[n];
		stereoPcm.sL[n] += filterSL[n];
		filterSR[n] = 0.0;
		filterSL[n] = 0.0;
	}

	IIR_resonator(frequency[2] / originalpcm_.fs, frequency[2] / bandwidth, aR, bR);/*IIRフィルタ設計*/
	IIR_resonator(frequency[2] / originalpcm_.fs, frequency[2] / bandwidth, aL, bL);/*IIRフィルタ設計*/
	IIR_Filtering(originalpcm_.sR, filterSR, originalpcm_.length, aR, bR, delayI, delayJ);/*フィルタリング*/
	IIR_Filtering(originalpcm_.sL, filterSL, originalpcm_.length, aL, bL, delayI, delayJ);/*フィルタリング*/
	for (int n = 0; n < stereoPcm.length; n++) {
		stereoPcm.sR[n] += filterSR[n];
		stereoPcm.sL[n] += filterSL[n];
		filterSR[n] = 0.0;
		filterSL[n] = 0.0;
	}

	IIR_resonator(frequency[3] / originalpcm_.fs, frequency[3] / bandwidth, aR, bR);/*IIRフィルタ設計*/
	IIR_resonator(frequency[3] / originalpcm_.fs, frequency[3] / bandwidth, aL, bL);/*IIRフィルタ設計*/
	IIR_Filtering(originalpcm_.sR, filterSR, originalpcm_.length, aR, bR, delayI, delayJ);/*フィルタリング*/
	IIR_Filtering(originalpcm_.sL, filterSL, originalpcm_.length, aL, bL, delayI, delayJ);/*フィルタリング*/
	for (int n = 0; n < stereoPcm.length; n++) {
		stereoPcm.sR[n] += filterSR[n];
		stereoPcm.sL[n] += filterSL[n];
		filterSR[n] = 0.0;
		filterSL[n] = 0.0;
	}
	f0;
	//IIR_resonator(frequency / monoPcm_->fs, frequency / bandwidth, aR, bR);
	//IIR_resonator(frequency / monoPcm_->fs, frequency / bandwidth, aL, bL);

	//std::vector<double> filterSR(monoPcm_->length, 0.0);
	//std::vector<double> filterSL(monoPcm_->length, 0.0);
	//IIR_Filtering(noizePcm_.sR, filterSR, monoPcm_->length, aR, bR, 2, 2); // noizePcm_ を使用してフィルタリング
	//IIR_Filtering(noizePcm_.sL, filterSL, monoPcm_->length, aL, bL, 2, 2); // noizePcm_ を使用してフィルタリング

	//for (int n = 0; n < monoPcm_->length; n++) {
	//	monoPcm_->sR[n] = filterSR[n];
	//	monoPcm_->sL[n] = filterSL[n];
	//}
	//f0;
}

void SoundWave::CreateSpeechVoice(STEREO_PCM& mosnoPcm,  const std::string& text) {

	double fade_factor = 0.98; // ディエンファシスの減衰率

	int total_length = int(mosnoPcm.fs * pronunciationTime_ * (text.size())); // 合計の長さを計算
	mosnoPcm.length = total_length;
	mosnoPcm.sR.resize(int(total_length), 0.0);
	mosnoPcm.sL.resize(int(total_length), 0.0);

	int current_position = 0; // 現在の位置を管理する変数

	double f0 = 100.0; // 基本周波数
	int length = int(mosnoPcm.length/text.size());
	STEREO_PCM copyMonoPcm_;
	copyMonoPcm_.fs = mosnoPcm.fs;
	copyMonoPcm_.bits = mosnoPcm.bits;
	copyMonoPcm_.length = length;
	copyMonoPcm_.sR.resize(length);
	copyMonoPcm_.sL.resize(length);

	for (char c : text) {
		std::map<char, std::pair<std::vector<double>, double>> formant_map;
		formant_map['a'] = { {800.0,1200,2500,3500}, 100.0 };  // あ
		formant_map['i'] = { {300,2300,2900,3500}, 100.0 }; // い
		formant_map['u'] = { {300,1200,2500,3500}, 100.0 }; // う
		formant_map['e'] = { {500,1900,2500,3500}, 100.0 }; // え
		formant_map['o'] = { {500,800,2500,3500}, 100.0 };  // お

		if (formant_map.find(c) != formant_map.end()) {
			std::vector<double> frequency(4); 
			for (int i = 0; i < frequency.size(); i++) {
				frequency [i] = formant_map[c].first[i];
			}
			double bandwidth = formant_map[c].second;
			WaveFilter(copyMonoPcm_, frequency, bandwidth, f0);

			// ディエンファシス
			std::vector<double> sR(length, 0.0);
			std::vector<double> sL(length, 0.0);
			sR[0] = copyMonoPcm_.sR[0];
			sL[0] = copyMonoPcm_.sL[0];
			for (int n = 1; n < length; n++) {
				sR[n] = copyMonoPcm_.sR[n] + fade_factor * sR[n - 1];
				sL[n] = copyMonoPcm_.sL[n] + fade_factor * sL[n - 1];
			}

			// pcm1_ の適切な位置に波形をコピー
			for (int n = 0; n < length; ++n) {
				mosnoPcm.sR[current_position + n] += sR[n];
				mosnoPcm.sL[current_position + n] += sL[n];
			}
			current_position += length; // 次の文字の位置を設定
		}
	}
}
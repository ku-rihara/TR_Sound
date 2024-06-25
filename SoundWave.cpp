#include "SoundWave.h"
#include<vector>

void SoundWave::Init() {
	stereoPcm_.fs = 44100;//標本化周波数
	stereoPcm_.bits = 16;//量子化精度
	stereoPcm_.length = int(stereoPcm_.fs * 2.2);//音データの長さ
	stereoPcm_.sL = (double*)calloc(stereoPcm_.length, sizeof(double));//音データ
	stereoPcm_.sR = (double*)calloc(stereoPcm_.length, sizeof(double));//音データ

	duration_ = 0.25;
	CreateWave();//波作成
	wave_write_16bit_stereo(&stereoPcm_, "CDE.wav");

}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	SineWave_Stereo(&stereoPcm_, 261.63, 0.7, duration_ * 0.0, duration_); /* C4 */
	SineWave_Stereo(&stereoPcm_, 293.66, 0.7, duration_ * 1.0, duration_); /* D4 */
	SineWave_Stereo(&stereoPcm_, 329.63, 0.7, duration_ * 2.0, duration_); /* E4 */
	SineWave_Stereo(&stereoPcm_, 349.23, 0.7, duration_ * 3.0, duration_); /* F4 */
	SineWave_Stereo(&stereoPcm_, 392.00, 0.7, duration_ * 4.0, duration_); /* G4 */
	SineWave_Stereo(&stereoPcm_, 440.00, 0.7, duration_ * 5.0, duration_); /* A4 */
	SineWave_Stereo(&stereoPcm_, 493.88, 0.7, duration_ * 6.0, duration_); /* B4 */
	SineWave_Stereo(&stereoPcm_, 523.25, 0.7, duration_ * 7.0, duration_); /* C5 */

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
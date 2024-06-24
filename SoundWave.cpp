#include "SoundWave.h"

void SoundWave::Init(){
	stereoPcm_.fs = 44100;//標本化周波数
	stereoPcm_.bits = 16;//量子化精度
	stereoPcm_.length = int(stereoPcm_.fs * 2.2);//音データの長さ
	stereoPcm_.sL = (double*)calloc(stereoPcm_.length, sizeof(double));//音データ
	stereoPcm_.sR = (double*)calloc(stereoPcm_.length, sizeof(double));//音データ


	CreateWave();//波作成
	wave_write_16bit_stereo(&stereoPcm_, "CDE.wav");

}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	SineWave_Stereo(&stereoPcm_, 261.63, 0.7, int(stereoPcm_.fs * 0.00), int(stereoPcm_.fs * 0.25)); /* C4 */
	SineWave_Stereo(&stereoPcm_, 293.66, 0.7, int(stereoPcm_.fs * 0.25), int(stereoPcm_.fs * 0.25)); /* D4 */
	SineWave_Stereo(&stereoPcm_, 329.63, 0.7, int(stereoPcm_.fs * 0.50), int(stereoPcm_.fs * 0.25)); /* E4 */
	SineWave_Stereo(&stereoPcm_, 349.23, 0.7, int(stereoPcm_.fs * 0.75), int(stereoPcm_.fs * 0.25)); /* F4 */
	SineWave_Stereo(&stereoPcm_, 392.00, 0.7, int(stereoPcm_.fs * 1.00), int(stereoPcm_.fs * 0.25)); /* G4 */
	SineWave_Stereo(&stereoPcm_, 440.00, 0.7, int(stereoPcm_.fs * 1.25), int(stereoPcm_.fs * 0.25)); /* A4 */
	SineWave_Stereo(&stereoPcm_, 493.88, 0.7, int(stereoPcm_.fs * 1.50), int(stereoPcm_.fs * 0.25)); /* B4 */
	SineWave_Stereo(&stereoPcm_, 523.25, 0.7, int(stereoPcm_.fs * 1.75), int(stereoPcm_.fs * 0.25)); /* C5 */

}

void SoundWave::WaveVisualize() {
	// 可視化のための座標取得
	int num_points = stereoPcm_.length / 60; // 100分割
	float* waveX = new float[num_points];
	float* waveY = new float[num_points];
	for (int k = 0; k < num_points; k++) {
		waveX[k] = float(k * 1280 / num_points);
		waveY[k] = float(360 + stereoPcm_.sL[k * 60] * 300); // Y座標を中央にシフトし、スケーリング
	}

	for (int k = 0; k < num_points - 1; k++) {
		Novice::DrawLine((int)waveX[k], (int)waveY[k], (int)waveX[k + 1], (int)waveY[k + 1], WHITE);
	}
}
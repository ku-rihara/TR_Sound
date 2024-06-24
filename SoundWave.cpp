#include "SoundWave.h"

void SoundWave::Init(){
	monoPcm_.fs = 44100;//標本化周波数
	monoPcm_.bits = 16;//量子化精度
	monoPcm_.length = int(monoPcm_.fs * 2.2);//音データの長さ
	monoPcm_.s = (double*)calloc(monoPcm_.length, sizeof(double));//音データ


	CreateWave();//波作成
	wave_write_16bit_mono(&monoPcm_, "CDE.wav");

}

void SoundWave::Update() {

}

void SoundWave::Draw() {

}

void SoundWave::CreateWave() {
	SineWave_Mono(&monoPcm_, 261.63, 0.5, int(monoPcm_.fs * 0.00), int(monoPcm_.fs * 0.25)); /* C4 */
	SineWave_Mono(&monoPcm_, 293.66, 0.5, int(monoPcm_.fs * 0.25), int(monoPcm_.fs * 0.25)); /* D4 */
	SineWave_Mono(&monoPcm_, 329.63, 0.5, int(monoPcm_.fs * 0.50), int(monoPcm_.fs * 0.25)); /* E4 */
	SineWave_Mono(&monoPcm_, 349.23, 0.5, int(monoPcm_.fs * 0.75), int(monoPcm_.fs * 0.25)); /* F4 */
	SineWave_Mono(&monoPcm_, 392.00, 0.5, int(monoPcm_.fs * 1.00), int(monoPcm_.fs * 0.25)); /* G4 */
	SineWave_Mono(&monoPcm_, 440.00, 0.5, int(monoPcm_.fs * 1.25), int(monoPcm_.fs * 0.25)); /* A4 */
	SineWave_Mono(&monoPcm_, 493.88, 0.5, int(monoPcm_.fs * 1.50), int(monoPcm_.fs * 0.25)); /* B4 */
	SineWave_Mono(&monoPcm_, 523.25, 0.5, int(monoPcm_.fs * 1.75), int(monoPcm_.fs * 0.25)); /* C5 */

}

void SoundWave::WaveVisualize() {
	// 可視化のための座標取得
	int num_points = monoPcm_.length / 200; // 100分割
	float* waveX = new float[num_points];
	float* waveY = new float[num_points];
	for (int k = 0; k < num_points; k++) {
		waveX[k] = float(k * 1280 / num_points);
		waveY[k] = float(360 + monoPcm_.s[k * 200] * 300); // Y座標を中央にシフトし、スケーリング
	}

	for (int k = 0; k < num_points - 1; k++) {
		Novice::DrawLine((int)waveX[k], (int)waveY[k], (int)waveX[k + 1], (int)waveY[k + 1], WHITE);
	}
}
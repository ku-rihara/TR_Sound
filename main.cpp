#include <Novice.h>
#include "wave.h"

const char kWindowTitle[] = "LC1A_12_クリハラ_ケイタ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	
	MONO_PCM pcm;
	int n, i;
	double f0, gain;

	pcm.fs = 44100;//
	pcm.bits = 16;
	pcm.length = pcm.fs * 1;
	pcm.s = (double*)calloc(pcm.length, sizeof(double));

	f0 = 500;//基本周波数

	//三角波
	for (i = 1; i <= 44; i += 2) {
		for (n = 0; n < pcm.length; n++) {
			pcm.s[n] += 1.0 / std::pow(i, 2) * sin(M_PI * i / 2.0) * sin(2.0 * M_PI * i * f0 * n / pcm.fs);
		}
	}
	gain = 0.1;//ゲイン
	for (n = 0; n < pcm.length; n++) {
		pcm.s[n] *= gain;
	}
	wave_write_16bit_mono(&pcm, "Box.wav");
	// 可視化のための座標取得
	int num_points = pcm.length / 100; // 100分割
	float* waveX = new float[num_points];
	float* waveY = new float[num_points];
	for (int k = 0; k < num_points; k++) {
		waveX[k] = float(k * 1280 / num_points);
		waveY[k] = float(360 + pcm.s[k * 100] * 300); // Y座標を中央にシフトし、スケーリング
	}
	free(pcm.s);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		for (int k = 0; k < num_points - 1; k++) {
			Novice::DrawLine((int)waveX[k], (int)waveY[k], (int)waveX[k + 1], (int)waveY[k + 1], WHITE);
		}
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

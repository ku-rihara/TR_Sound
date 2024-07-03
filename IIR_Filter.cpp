#include "IIR_Filter.h"
#include"wave.h"

void IIR_LPF(const double& fc, const double& Q, std::vector<double>& a, std::vector<double>& b) {
	const double cutoffFrequency = tan(M_PI * fc) / (2.0 * M_PI);//遮断周波数
	a[0] = 1.0 + 2.0 * M_PI * cutoffFrequency / Q + 4.0 * std::pow(M_PI, 2) * std::pow(fc, 2);
	a[1] = (8.0 * std::pow(M_PI, 2) * std::pow(fc, 2) - 2.0) / a[0];
	a[2] = (1.0 - 2.0 * M_PI * fc / Q + 4.0 * std::pow(M_PI, 2) * std::pow(fc, 2)) / a[0];
	b[0] = 4.0 * std::pow(M_PI, 2) * std::pow(fc, 2) / a[0];
	b[1] = 8.0 * std::pow(M_PI, 2) * std::pow(fc, 2) / a[0];
	b[2] = 4.0 * std::pow(M_PI, 2) * std::pow(fc, 2) / a[0];

	a[0] = 1.0;
}
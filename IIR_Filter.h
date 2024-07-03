#pragma once
#include<vector>

/// <summary>/// 
/// 
/// </summary>/// 
/// <param name="遮断周波数"></param>/// 
/// <param name="Q(クオリティファクタ)"></param>/// 
/// <param name="a"></param>///
/// <param name="b"></param>///
/// <returns></returns>
void IIR_LPF(const double& fc, const double& Q,  std::vector<double>& a,  std::vector<double>& b);

//// <summary>/// 
/// 
/// </summary>/// 
/// <param name="遮断周波数"></param>/// 
/// <param name="Q(クオリティファクタ)"></param>/// 
/// <param name="a"></param>///
/// <param name="b"></param>///
/// <returns></returns>
void IIR_HPF(const double& fc, const double& Q, std::vector<double>& a, std::vector<double>& b);


//// <summary>/// 
/// 
/// </summary>/// 
/// <param name="遮断周波数(低い)"></param>/// 
/// <param name="遮断周波数(高い)"></param>/// 
/// <param name="a"></param>///
/// <param name="b"></param>///
/// <returns></returns>
void IIR_BPF(const double& fcLow, const double& fcHight, std::vector<double>& a, std::vector<double>& b);

#pragma once
#include<vector>

/// <summary>/// 
/// 
/// </summary>/// 
/// <param name="fc"></param>/// 
/// <param name="Q"></param>/// 
/// <param name="a"></param>///
/// <param name="b"></param>///
/// <returns></returns>
void IIR_LPF(const double& fc, const double& Q,  std::vector<double>& a,  std::vector<double>& b);

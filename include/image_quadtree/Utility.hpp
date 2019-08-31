/**
 * @file    Utility.hpp
 *
 * @author  btran
 *
 * @date    2019-08-31
 *
 * Copyright (c) organization
 *
 */

#pragma once

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include <opencv2/opencv.hpp>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif  // M_PI

namespace pcv
{
inline std::pair<std::vector<double>, std::vector<double>> generateHeartShapePoints(const size_t numPoints = 300)
{
    std::vector<double> vx, vy;
    const double dt = 2.0 * M_PI / numPoints;

    for (double t = 0.0; t <= 2.0 * M_PI; t += dt) {
        vx.emplace_back(16.0 * sin(t) * sin(t) * sin(t));
        vy.emplace_back(-13.0 * cos(t) + 5.0 * cos(2.0 * t) + 2.0 * cos(3.0 * t) + cos(4.0 * t));
    }

    const auto vxMinMax = std::minmax_element(vx.begin(), vx.end());
    const double vxMin = *vxMinMax.first;
    const double vxMax = *vxMinMax.second;
    const double vxDist = vxMax - vxMin;

    const auto vyMinMax = std::minmax_element(vy.begin(), vy.end());
    const double vyMin = *vyMinMax.first;
    const double vyMax = *vyMinMax.second;
    const double vyDist = vyMax - vyMin;

    for (double& elem : vx) {
        elem = (elem - vxMin) / vxDist;
    }

    for (double& elem : vy) {
        elem = (elem - vyMin) / vyDist;
    }

    return std::make_pair(vx, vy);
}

void drawHeartShape(cv::Mat& img, const cv::Point& p1, const cv::Point& p2, const cv::Scalar& color);

}  // namespace pcv

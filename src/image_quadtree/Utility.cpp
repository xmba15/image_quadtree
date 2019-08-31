/**
 * @file    Utility.cpp
 *
 * @author  bt
 *
 * @date    2019-08-31
 *
 * Copyright (c) organization
 *
 */

#include "image_quadtree/Utility.hpp"

namespace pcv
{
void drawHeartShape(cv::Mat& img, const cv::Point& p1, const cv::Point& p2, const cv::Scalar& color)
{
    cv::Point rectSize = p2 - p1;
    const size_t MAX_WIDTH = std::max(rectSize.x, rectSize.y);
    const size_t MIN_WIDTH = std::max(rectSize.x, rectSize.y);
    const size_t offsetX = (rectSize.x - MIN_WIDTH) / 2;
    const size_t offsetY = (rectSize.y - MIN_WIDTH) / 2;

    const cv::Point offsetPoint = p1 + cv::Point(offsetX, offsetY);

    auto points = pcv::generateHeartShapePoints();
    std::vector<double> pointXs = points.first;
    std::vector<double> pointYs = points.second;

    std::vector<cv::Point> cvPoints;
    cvPoints.reserve(pointXs.size());

    for (size_t i = 0; i < pointXs.size(); ++i) {
        cv::Point curP = offsetPoint + cv::Point(MAX_WIDTH * pointXs[i], MAX_WIDTH * pointYs[i]);
        cvPoints.emplace_back(curP);
    }

    std::vector<std::vector<cv::Point>> pp{cvPoints};
    cv::fillPoly(img, pp, color);
}
}  // namespace pcv

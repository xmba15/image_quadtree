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
    assert(p1.x <= p2.x && p1.y <= p2.y);
    cv::Point rectSize;
    rectSize.x = p2.x - p1.x;
    rectSize.y = p2.y - p1.y;

    const int MAX_WIDTH = std::max(rectSize.x, rectSize.y);
    const int MIN_WIDTH = std::min(rectSize.x, rectSize.y);

    const int offsetX = (rectSize.x - MIN_WIDTH) / 2;
    const int offsetY = (rectSize.y - MIN_WIDTH) / 2;
    const cv::Point offsetPoint(p1.x + offsetX, p1.y + offsetY);

    auto points = pcv::generateHeartShapePoints();
    std::vector<double> pointXs = points.first;
    std::vector<double> pointYs = points.second;

    std::vector<cv::Point> cvPoints;
    cvPoints.reserve(pointXs.size());

    for (size_t i = 0; i < pointXs.size(); ++i) {
        cv::Point curP(offsetPoint.x + MIN_WIDTH * pointXs[i], offsetPoint.y + MIN_WIDTH * pointYs[i]);
        cvPoints.emplace_back(curP);
    }

    std::vector<std::vector<cv::Point>> pp{cvPoints};
    cv::fillPoly(img, pp, color);
}
}  // namespace pcv

/**
 * @file    ExVideoQuadtreeHeartShape.cpp
 *
 * @author  btran
 *
 * @date    2019-09-11
 *
 * Copyright (c) organization
 *
 */

#include <iostream>

#include <image_quadtree/image_quadtree.hpp>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
#ifdef IMAGE_PATH
    std::stringstream ss;
    ss << IMAGE_PATH << "/tokyo_tower.mp4";

    cv::VideoCapture cap(ss.str());
    cap.set(CV_CAP_PROP_FORMAT, CV_8UC3);

    if (!cap.isOpened()) {
        std::cout << "Failed to load the video"
                  << "\n";
        return EXIT_FAILURE;
    }

    int i = 0;

    while (cap.isOpened()) {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        pcv::ImageQuadtree imgQt(frame, 10, 1);
        std::stringstream namess;
        namess << "./frames/frame" << i << ".jpg";

        cv::Mat toWrite(frame.rows, frame.cols, CV_8UC3, cv::Scalar(0));
        imgQt.unpackcvMatArtQuadtree(toWrite, pcv::ImageQuadtree::ART_MODE::HEART_SHAPE);
        cv::imwrite(namess.str(), toWrite);

        ++i;
    }

    return EXIT_SUCCESS;
#endif  // IMAGE_PATH

    return EXIT_FAILURE;
}

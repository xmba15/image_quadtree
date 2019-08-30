/**
 * @file    ExImageQuadtree.cpp
 *
 * @author  btran
 *
 * @date    2019-08-31
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
    ss << IMAGE_PATH << "/baymax.jpg";
    pcv::ImageQuadtree imgQt(ss.str(), 10);

    cv::imwrite("quadtree_image.jpg", imgQt.unpackcvMatQuadtree());

#endif  // IMAGE_PATH
    return 0;
}
